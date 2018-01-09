#include "CameraSplineController.h"
#include "TransformationNode.h"
#include "ColladaImporter.h"
#include "GeometryNode.h"
#include <spline_library/spline.h>
#include <spline_library/vector.h>
#include <spline_library/splines/uniform_cr_spline.h>
#include <stdlib.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

glm::quat rotation_between_vectors(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	const float cos_theta = dot(start, dest);
	glm::vec3 rotation_axis;

	if (cos_theta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotation_axis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotation_axis) < 0.01) // bad luck, they were parallel, try again!
			rotation_axis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotation_axis = normalize(rotation_axis);
		return glm::angleAxis(glm::radians(180.0f), rotation_axis);
	}

	rotation_axis = cross(start, dest);

	float s = sqrt((1 + cos_theta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotation_axis.x * invs,
		rotation_axis.y * invs,
		rotation_axis.z * invs
	);
}

CameraSplineController::CameraSplineController(std::string name, TransformationNode* target, GroupNode *visualizer_container) : AnimatorNode(name)
{
	this->target_ = target;
	this->progress_ = 0.0;
	this->current_segment_ = 1;
	this->position_spline_ = nullptr;
	this->duration_ = 0.0;
	this->total_arc_length_ = 0;

#ifdef VISUALIZE_KEYPOINTS
	this->keypoint_visualizer_ = MeshResource::create_cube(glm::vec3(0, 0, 1));
	this->tween_visualizer_ = MeshResource::create_cube(glm::vec3(0, 1, 0));
	this->cam_visualizer_ = MeshResource::create_cube(glm::vec3(1, 0, 0));
	this->visualizer_container_ = visualizer_container;
	this->test_cam_ = nullptr;
#endif

}

CameraSplineController::~CameraSplineController()
{
}

void CameraSplineController::add_keypoint(const KeyPoint& key_point)
{
	this->keypoints_.push_back(key_point);
}

void CameraSplineController::update(double delta)
{
	// position
	float tween = this->progress_ / this->duration_ * this->position_spline_->getMaxT();
	const Vector3 interpolated_pos = this->position_spline_->getPosition(tween);
	const glm::vec3 new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);
	
	// rotation
	KeyPoint current_keypoint = this->keypoints_[int(tween + 1)];
	int next_keypoint_index = (current_keypoint.at_time + current_keypoint.duration) / this->duration_ * this->position_spline_->getMaxT();
	glm::vec3 target_look_at = this->keypoints_[int(next_keypoint_index) + 1].look_at_pos;

	glm::vec3 forward = glm::normalize(glm::transpose(get_target()->get_inverse_transformation()) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	glm::mat4 current_rotation = glm::inverse(glm::lookAt(glm::vec3(0, 0, 0), forward, glm::vec3(0, 1, 0)));
	glm::vec3 target_direction = target_look_at - new_pos;
	glm::mat4 new_rotation;
	if (glm::length(target_direction) >= 0.00001)
	{
		glm::mat4 target_rotation = glm::inverse(glm::lookAt(glm::vec3(0, 0, 0), target_direction, glm::vec3(0, 1, 0)));

		auto rotation_tween = (this->progress_ - this->keypoints_[int(tween) + 1].at_time) / this->keypoints_[int(tween) + 1].duration;
		new_rotation = glm::interpolate(current_rotation, target_rotation, float(rotation_tween));

	} else
	{
		new_rotation = current_rotation;
	}
	
	glm::mat4 mat = glm::translate(new_pos) * new_rotation;
	get_target()->set_transformation(mat);


	if (this->progress_ < this->duration_) {
		this->progress_ += delta;
	}
	this->progress_ = std::min(this->progress_, this->duration_);

	if (glfwGetKey(this->get_rendering_engine()->get_window(), GLFW_KEY_M))
	{
		this->progress_ = 0;
		this->current_segment_ = 1;
	}
}

void CameraSplineController::init(RenderingEngine* rendering_engine)
{
	AnimatorNode::init(rendering_engine);
	const auto interpolated_pos = position_spline_->getPosition(0);
	const glm::vec3 new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);
	auto mat = glm::lookAt(new_pos, this->keypoints_[1].look_at_pos, glm::vec3(0, 1, 0));
	get_target()->set_transformation(glm::inverse(mat), mat);

#ifdef VISUALIZE_KEYPOINTS
	this->keypoint_visualizer_->init();
	rendering_engine->register_resource(this->keypoint_visualizer_);
	this->tween_visualizer_->init();
	rendering_engine->register_resource(this->tween_visualizer_);
	this->cam_visualizer_->init();
	rendering_engine->register_resource(this->cam_visualizer_);
#endif
}

void CameraSplineController::build_spline()
{
	std::cout << "Building Spline..." << std::endl;

	this->duration_ = 0;
	std::vector<Vector3> timeless_position_spline_points;
	for (auto &point : this->keypoints_)
	{
		point.at_time = this->duration_;
		this->duration_ += point.duration;

		timeless_position_spline_points.push_back(Vector3({ point.pos.x, point.pos.y, point.pos.z }));
	}
	// remove both of the last 2 points
	auto last = this->keypoints_[this->keypoints_.size() - 1];
	auto nexttolast = this->keypoints_[this->keypoints_.size() - 2];
	auto first = this->keypoints_[0];
	this->duration_ -= first.duration + last.duration + nexttolast.duration;

	UniformCRSpline<Vector3> timeless_position_spline = UniformCRSpline<Vector3>(timeless_position_spline_points);

	std::vector<KeyPoint> new_keypoints;
	new_keypoints.push_back(first);
	std::vector<Vector3> position_spline_points;
	position_spline_points.push_back(Vector3({ this->keypoints_[0].pos.x, this->keypoints_[0].pos.y, this->keypoints_[0].pos.z }));
	for (int i = 0; i < timeless_position_spline.getMaxT(); i++)
	{
		position_spline_points.push_back(timeless_position_spline.getPosition(i));
		auto keypoint = this->keypoints_[i + 1];
		new_keypoints.push_back(keypoint);
		for (int time = 1; time < keypoint.duration; time++)
		{ 
			auto tweened_pos = timeless_position_spline.getPosition(i + float(time) / keypoint.duration);
			position_spline_points.push_back(tweened_pos);

			auto kp = keypoint;
			kp.pos = glm::vec3(tweened_pos[0], tweened_pos[1], tweened_pos[2]);
			new_keypoints.push_back(kp);
		}
	}
	position_spline_points.push_back(Vector3({ nexttolast.pos.x, nexttolast.pos.y, nexttolast.pos.z }));
	new_keypoints.push_back(nexttolast);
	position_spline_points.push_back(Vector3({ last.pos.x, last.pos.y, last.pos.z }));
	new_keypoints.push_back(last);
	this->keypoints_ = new_keypoints;
	this->current_look_at_ = this->keypoints_[1].look_at_pos;

	position_spline_ = new UniformCRSpline<Vector3>(position_spline_points);
	total_arc_length_ = position_spline_->totalLength();

#ifdef VISUALIZE_KEYPOINTS
	for (auto &point : this->keypoints_)
	{
		GeometryNode *node = new GeometryNode("keypoint" + std::to_string(this->keypoints_.size()), this->keypoint_visualizer_);
		node->set_transformation(glm::translate(point.pos) * glm::scale(glm::vec3(0.25, 0.25, 0.25)));
		this->visualizer_container_->add_node(node);
	}

	/*float progress = 0;
	float max_t = position_spline_->getMaxT();
	while (progress < max_t)
	{
		auto interpolated_pos = this->position_spline_->getPosition(progress);
		const auto new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);

		GeometryNode *node = new GeometryNode("keypoint" + std::to_string(this->keypoints_.size()), this->tween_visualizer_);
		node->set_transformation(glm::translate(new_pos) * glm::scale(glm::vec3(0.1, 0.1, 0.1)));
		this->visualizer_container_->add_node(node);

		progress += 1.0 / total_arc_length_;
	}*/
	
	
	this->test_cam_ = new GeometryNode("keypoint", this->keypoint_visualizer_);
	this->visualizer_container_->add_node(this->test_cam_);

#endif
	std::cout << "Finished building Spline..." << std::endl;
}

TransformationNode* CameraSplineController::get_target() const
{
#ifdef VISUALIZE_KEYPOINTS
	return test_cam_ ? test_cam_ : this->target_;
#else
		return this->target_;
#endif
}