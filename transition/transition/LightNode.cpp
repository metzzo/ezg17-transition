#include "LightNode.h"
#include "ILightShader.h"


LightNode::LightNode(const std::string& name): RenderingNode(name, glm::ivec2(), glm::mat4())
{
	this->linear_ = 0;
	this->quadratic_ = 0;
	this->is_shadow_casting_ = false;
}

LightNode::~LightNode()
{
}

void LightNode::set_color(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular)
{
	this->ambient_ = ambient;
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

void LightNode::apply_to_shader(const ILightShader* shader, const int light_index)
{
	shader->set_light_ambient(light_index, this->ambient_);
	shader->set_light_diffuse(light_index, this->diffuse_);
	shader->set_light_specular(light_index, this->specular_);

	shader->set_light_linear(light_index, this->linear_);
	shader->set_light_quadratic(light_index, this->quadratic_);
}

