#include "LightNode.h"
#include "ILightShader.h"


LightNode* LightNode::createDirectionalLight(const std::string& name, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction) {
	LightNode* n = new LightNode(name, DIRECTIONAL_LIGHT);
	n->set_color(diffuse, specular);
	n->direction_ = direction;
	return n;
}

LightNode* LightNode::createPointLight(const std::string& name, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position, const glm::vec3& attenuation) {
	LightNode* n = new LightNode(name, POINT_LIGHT);
	n->set_color(diffuse, specular);
	n->set_attenuation(attenuation.x, attenuation.y, attenuation.z);
	n->set_transformation(glm::translate(position), glm::translate(-position));
	return n;
}

LightNode::LightNode(const std::string& name, LightType light_type): RenderingNode(name, glm::ivec2(), glm::mat4())
{
	this->linear_ = 0;
	this->quadratic_ = 0;
	this->constant_ = 0;
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

void LightNode::set_attenuation(const float constant, const float linear, const float quadratic)
{
	this->constant_ = constant;
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

}

void LightNode::apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation)
{
	TransformationNode::apply_transformation(transformation, inverse_transformation);
}
