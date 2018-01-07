#include "CameraSplineController.h"
#include "TransformationNode.h"
#include "ColladaImporter.h"
#include "GeometryNode.h"
#include <spline_library/spline.h>
#include <spline_library/vector.h>
#include <spline_library/splines/uniform_cr_spline.h>

#define VISUALIZE_KEYPOINTS

CameraSplineController::CameraSplineController(std::string name, TransformationNode* target, GroupNode *visualizer_container) : AnimatorNode(name)
{
	this->target_ = target;
	this->progress_ = 0.0;

	this->visualizer_ = MeshResource::create_cube(glm::vec3(0,0,1));
	this->visualizer_container_ = visualizer_container;

	this->test_cam_ = new GeometryNode("keypoint", this->visualizer_);
	this->visualizer_container_->add_node(this->test_cam_);
}

CameraSplineController::~CameraSplineController()
{
}

void CameraSplineController::add_keypoint(const KeyPoint& key_point)
{
	this->keypoints_.push_back(key_point);

#ifdef VISUALIZE_KEYPOINTS
	GeometryNode *node = new GeometryNode("keypoint" + std::to_string(this->keypoints_.size()), this->visualizer_);
	node->set_transformation(glm::translate(key_point.pos));
	this->visualizer_container_->add_node(node);
#endif
}

void CameraSplineController::update(double delta)
{
	auto interpolated_pos = this->position_spline_->getPosition(this->progress_);
	auto new_pos = glm::vec3(interpolated_pos[0], interpolated_pos[1], interpolated_pos[2]);

	this->test_cam_->set_transformation(glm::translate(new_pos));

	if (this->progress_ < this->position_spline_->getMaxT()) {
		this->progress_ += 0.75 * delta;
	}

	std::cout << this->progress_ << std::endl;

	if (glfwGetKey(this->get_rendering_engine()->get_window(), GLFW_KEY_B))
	{
		this->progress_ = 0;
	}
}

void CameraSplineController::init(RenderingEngine* rendering_engine)
{
	AnimatorNode::init(rendering_engine);

	this->visualizer_->init();
	rendering_engine->register_resource(this->visualizer_);

	// mark last
	auto last_key = static_cast<GeometryNode*>(this->visualizer_container_->find_by_name("keypoint" + std::to_string(this->keypoints_.size())));
	last_key->apply_transformation(Transformation::scale(glm::vec3(3,3,3)));

	// init spline
	std::vector<Vector3> spline_points;
	for (auto &point : this->keypoints_)
	{
		spline_points.push_back(Vector3({ point.pos.x, point.pos.y, point.pos.z }));
	}

	position_spline_ = new UniformCRSpline<Vector3>(spline_points);
}
