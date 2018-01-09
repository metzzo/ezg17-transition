#include "CameraSplineController.h"
#include "TransformationNode.h"
#include "ColladaImporter.h"
#include "GeometryNode.h"
#include <spline_library/spline.h>
#include <spline_library/vector.h>
#include <spline_library/splines/uniform_cr_spline.h>
#include <spline_library/utils/arclength.h>
#include <stdlib.h>
#include <glm/gtx/spline.hpp>
#include <glm/gtc/vec1.hpp> 


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
	const auto interpolated_pos = this->position_spline_->getPosition(this->progress_ / this->duration_ * this->position_spline_->getMaxT());
	const glm::vec3 new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);
	
#ifdef VISUALIZE_KEYPOINTS
	this->test_cam_->set_transformation(glm::translate(new_pos));
#endif

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
}
