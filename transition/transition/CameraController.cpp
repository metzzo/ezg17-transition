#include "CameraController.h"

CameraController::CameraController(const std::string name, CameraNode *camera, RenderingEngine *engine) : AnimatorNode(name)
{
	this->engine_ = engine;
	this->camera_ = camera;
}

void CameraController::update(double delta)
{
	glm::vec3 viewdirection = glm::transpose(camera_->get_view_matrix()) * glm::vec4(0, 0, -1, 0);
	glm::vec3 sidedirection = glm::cross(viewdirection, glm::vec3(0, 1, 0));
	viewdirection.y = 0;
	sidedirection.y = 0;
	viewdirection = glm::normalize(viewdirection);
	sidedirection = glm::normalize(sidedirection);
	GLFWwindow* window = engine_->get_window();
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera_->apply_transformation(Transformation::translate(viewdirection*speed_*(float)delta));
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) {
		camera_->apply_transformation(Transformation::translate(viewdirection*-speed_*(float)delta));
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera_->apply_transformation(Transformation::translate(sidedirection*-speed_*(float)delta));
	}
	else if (glfwGetKey(window, GLFW_KEY_D)) {
		camera_->apply_transformation(Transformation::translate(sidedirection*speed_*(float)delta));
	}
	double x = 0, y = 0;
	glfwGetCursorPos(window, &x, &y);
	double dX = cursorX_ - x;
	double dY = cursorY_ - y;
	cursorX_ = x;
	cursorY_ = y;
	auto cpos = camera_->get_position();
	if (abs(verAngle_ + dY) <= 120) {
		verAngle_ += dY;
		camera_->apply_transformation(Transformation::rotate_around_point(dY*sensitivity_, sidedirection, cpos));
	}
	camera_->apply_transformation(Transformation::rotate_around_point(dX*sensitivity_, glm::vec3(0, 1, 0), cpos));
}
