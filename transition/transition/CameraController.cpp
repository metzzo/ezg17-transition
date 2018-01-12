#include "CameraController.h"
#include "glheaders.h"
#include "RenderingEngine.h"
#include "TransformationNode.h"
#include <fstream>
#include "MeshResource.h"
#include "GroupNode.h"
#include "GeometryNode.h"

CameraController::CameraController(const std::string name, TransformationNode *target, GroupNode *container) : AnimatorNode(name)
{
	this->target_ = target;
	this->n_pressed_ = false;

	this->mesh_ = MeshResource::create_cube(glm::vec3(0, 1, 0));
	this->look_at_marker_ = new GeometryNode("keypoint", mesh_);
	container->add_node(this->look_at_marker_);
}

void CameraController::update(double delta)
{
	GLFWwindow* window = get_rendering_engine()->get_window();
	if (glfwGetKey(window, GLFW_KEY_L)) {
		std::cout << "Position " << this->target_->get_position().x << " " << this->target_->get_position().y << " " << this->target_->get_position().z << std::endl;
	}
	glm::vec3 viewdirection = glm::transpose(target_->get_inverse_transformation()) * glm::vec4(0, 0, -1, 0);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		n_pressed_ = true;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE && n_pressed_)
	{
		std::cout << "Swag" << std::endl;
		n_pressed_ = false;
		glm::vec3 pos = this->target_->get_position();
		glm::vec3 lookat = this->look_at_marker_->get_position();
		std::ofstream file;
		file.open("cam_position.txt", std::ios::app);
		file << "cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(" << pos.x << ", " << pos.y << ", " << pos. z <<
			"), glm::vec3(" << lookat.x << ", " << lookat.y << ", " << lookat.z <<
			"), 0));\n";
		file.close();
	}
	for (int i = 0; i < 10; i++) {
		if (glfwGetKey(window, GLFW_KEY_F1 + i))
		{
			if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))
			{
				std::cout << "Restored from " << i << std::endl;
				this->target_->set_transformation(this->save_trafo_[i]);
			} else
			{
				std::cout << "Save to " << i << std::endl;
				this->save_trafo_[i] = this->target_->get_transformation();
			}
			
		}
	}

	/*if (glfwGetKey(window, GLFW_KEY_C))
	{
		look_at_marker_->set_transformation(glm::translate(this->target_->get_position()));
	}

	if (glfwGetKey(window, GLFW_KEY_V))
	{
		auto mat = glm::inverse(glm::lookAt(this->target_->get_position(), look_at_marker_->get_position(), glm::vec3(0, 1, 0)));
		target_->set_transformation(mat);
	}*/


	glm::vec3 sidedirection = glm::cross(viewdirection, glm::vec3(0, 1, 0));
	viewdirection.y = 0;
	sidedirection.y = 0;
	viewdirection = glm::normalize(viewdirection);
	sidedirection = glm::normalize(sidedirection);
	float currentspeed = speed_;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		currentspeed /= 4;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		currentspeed *= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		target_->apply_transformation(Transformation::translate(viewdirection*currentspeed*(float)delta));
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) {
		target_->apply_transformation(Transformation::translate(viewdirection*-currentspeed*(float)delta));
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		target_->apply_transformation(Transformation::translate(sidedirection*-currentspeed*(float)delta));
	}
	else if (glfwGetKey(window, GLFW_KEY_D)) {
		target_->apply_transformation(Transformation::translate(sidedirection*currentspeed*(float)delta));
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		target_->apply_transformation(Transformation::translate(glm::vec3(0, currentspeed*(float)delta, 0)));
	}
	else if (glfwGetKey(window, GLFW_KEY_Q)) {
		target_->apply_transformation(Transformation::translate(glm::vec3(0, -currentspeed*(float)delta, 0)));
	}
	double x = 0, y = 0;
	glfwGetCursorPos(window, &x, &y);
	double dX = cursorX_ - x;
	double dY = cursorY_ - y;
	cursorX_ = x;
	cursorY_ = y;
	auto cpos = target_->get_position();
	if (abs(verAngle_ + dY) <= 120) {
		verAngle_ += dY;
		target_->apply_transformation(Transformation::rotate_around_point(dY*sensitivity_, sidedirection, cpos));
	}
	target_->apply_transformation(Transformation::rotate_around_point(dX*sensitivity_, glm::vec3(0, 1, 0), cpos));
}

void CameraController::init(RenderingEngine* rendering_engine)
{
	AnimatorNode::init(rendering_engine);

	this->mesh_->init();
	rendering_engine->register_resource(this->mesh_);
}
