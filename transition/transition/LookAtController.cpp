#include "LookAtController.h"
#include "TransformationNode.h"
#include "RenderingEngine.h"
#include <GLFW/glfw3.h>

LookAtController::LookAtController(const std::string name, TransformationNode* target_node,
	TransformationNode* source_node) : AnimatorNode(name)
{

	this->target_node_ = target_node;
	this->source_node_ = source_node;
}

void LookAtController::update(double delta)
{
	if (!glfwGetKey(get_rendering_engine()->get_window(), GLFW_KEY_SPACE))
	{
		return;
	}

	auto target_pos = target_node_->get_position();
	target_pos.y = 0;
	auto origin_pos = source_node_->get_position();
	auto mat = glm::inverse(glm::lookAt(origin_pos, target_pos, glm::vec3(1, 1, 0)));
	source_node_->set_transformation(mat);
}

