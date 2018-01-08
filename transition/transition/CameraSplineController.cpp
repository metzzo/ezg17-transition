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
	const KeyPoint keypoints[4] = {
		this->keypoints_[this->current_segment_ - 1],
		this->keypoints_[this->current_segment_],
		this->keypoints_[this->current_segment_ + 1],
		this->keypoints_[this->current_segment_ + 2]
	};


	const auto start_time = keypoints[1].at_time - this->keypoints_[0].duration;
	const auto tween = (this->progress_ - start_time) / keypoints[1].duration;

	auto interpolated_time = glm::catmullRom(glm::vec1(keypoints[0].at_time), glm::vec1(keypoints[1].at_time), glm::vec1(keypoints[2].at_time), glm::vec1(keypoints[3].at_time), float(tween)).x;
	
	auto last = this->keypoints_.size() - 1;
	auto tmp = glm::catmullRom(glm::vec1(keypoints[0].at_time), glm::vec1(keypoints[1].at_time), glm::vec1(keypoints[2].at_time), glm::vec1(keypoints[3].at_time), 1.0).x;
	auto tmp_interpolated = tmp / this->duration_ * this->position_spline_->getMaxT();

	auto i = 1;
	while (i  < this->current_segment_)
	{
		tmp -= this->keypoints_[i].duration;
		interpolated_time -= this->keypoints_[i].duration;
	}

	const auto position_at = float(this->current_segment_ + interpolated_time / keypoints[3].duration);
	const auto interpolated_pos = this->position_spline_->getPosition(position_at);
	const glm::vec3 new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);
	
	std::cout << "Pr: " << this->progress_ << " Po: " << position_at << " Dur: " << keypoints[1].duration << " Tw: " << interpolated_time << std::endl;

	this->current_segment_ = std::min(this->current_segment_ + floor(tween), this->keypoints_.size() - 4.0);

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
	std::vector<Vector3> position_spline_points;
	for (auto &point : this->keypoints_)
	{
		point.at_time = this->duration_;
		this->duration_ += point.duration;

		position_spline_points.push_back(Vector3({ point.pos.x, point.pos.y, point.pos.z }));
	}

	// remove both of the last 2 points
	this->duration_ -= this->keypoints_[0].duration + this->keypoints_[this->keypoints_.size() - 1].duration + this->keypoints_[this->keypoints_.size() - 2].duration;

	position_spline_ = new UniformCRSpline<Vector3>(position_spline_points);
	total_arc_length_ = position_spline_->totalLength();

#ifdef VISUALIZE_KEYPOINTS
	for (auto &point : this->keypoints_)
	{
		GeometryNode *node = new GeometryNode("keypoint" + std::to_string(this->keypoints_.size()), this->keypoint_visualizer_);
		node->set_transformation(glm::translate(point.pos) * glm::scale(glm::vec3(0.25, 0.25, 0.25)));
		this->visualizer_container_->add_node(node);
	}

	float progress = 0;
	float max_t = position_spline_->getMaxT();
	while (progress < max_t)
	{
		auto interpolated_pos = this->position_spline_->getPosition(progress);
		const auto new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);

		GeometryNode *node = new GeometryNode("keypoint" + std::to_string(this->keypoints_.size()), this->tween_visualizer_);
		node->set_transformation(glm::translate(new_pos) * glm::scale(glm::vec3(0.1, 0.1, 0.1)));
		this->visualizer_container_->add_node(node);

		progress += 1.0 / total_arc_length_ * 10;
	}
	
	
	this->test_cam_ = new GeometryNode("keypoint", this->keypoint_visualizer_);
	this->visualizer_container_->add_node(this->test_cam_);

#endif
}
