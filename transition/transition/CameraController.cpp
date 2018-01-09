#include "CameraController.h"
#include "glheaders.h"
#include "RenderingEngine.h"
#include "TransformationNode.h"
#include <fstream>

CameraController::CameraController(const std::string name, TransformationNode *target) : AnimatorNode(name)
{
	this->target_ = target;
	this->n_pressed_ = false;
}

void CameraController::update(double delta)
{
	GLFWwindow* window = get_rendering_engine()->get_window();
	if (glfwGetKey(window, GLFW_KEY_L)) {
		std::cout << "Position " << this->target_->get_position().x << " " << this->target_->get_position().y << " " << this->target_->get_position().z << std::endl;
	}
	glm::vec3 viewdirection = glm::transpose(target_->get_inverse_transformation()) * glm::vec4(0, 0, -1, 0);
	
	if (glfwGetKey(window, GLFW_KEY_N) && !n_pressed_)
	{
		n_pressed_ = true;
		glm::vec3 pos = this->target_->get_position();
		std::ofstream file;
		file.open("cam_position.txt", std::ios::app);
		file << "cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(" << pos.x << ", " << pos.y << ", " << pos. z <<
			"), glm::vec3(0, 0, 0), 0));\n";
		file.close();
	} else
	{
		n_pressed_ = false;
	}

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
