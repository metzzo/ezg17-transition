#include "AnimatorNode.h"

void AnimatorNode::apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation)
{
	// do nothing with trafo
}

std::vector<AnimatorNode*> AnimatorNode::get_animator_nodes()
{
	return{ this };
}
