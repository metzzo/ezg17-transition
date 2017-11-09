#include "LightNode.h"
#include "ILightShader.h"


LightNode::LightNode(const std::string& name, LightType light_type): RenderingNode(name, glm::ivec2(), glm::mat4())
{
	this->linear_ = 0;
	this->quadratic_ = 0;
	this->is_shadow_casting_ = false;
	this->light_type_ = light_type;
}

LightNode::~LightNode()
{
}

void LightNode::set_color(const glm::vec3 diffuse, const glm::vec3 specular)
{
	this->diffuse_ = diffuse;
	this->specular_ = specular;
}

void LightNode::set_params(const float linear, const float quadratic)
{
	this->linear_ = linear;
	this->quadratic_ = quadratic;
}

std::vector<RenderingNode*> LightNode::get_rendering_nodes()
{
	if (this->is_shadow_casting_)
	{
		return{ this };
	} else
	{
		return std::vector<RenderingNode*>();
	}
}

std::vector<LightNode*> LightNode::get_light_nodes()
{
	return{ this };
}

void LightNode::set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo)
{
	TransformationNode::set_transformation(trafo, itrafo);
	// not sure whether this is correct
	this->direction_ = glm::transpose(itrafo)*glm::vec4(-1, 0, 0, 1);

}

void LightNode::apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation)
{
	TransformationNode::apply_transformation(transformation, inverse_transformation);
	this->direction_ = transformation * glm::vec4(this->direction_, 1.0);
}
