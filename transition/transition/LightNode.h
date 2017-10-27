#pragma once
#include <glm/glm.hpp>
#include "RenderingNode.h"
class ILightShader;

class LightNode :
	public RenderingNode
{
protected:
	glm::vec3 diffuse_;
	glm::vec3 specular_;

	float linear_;
	float quadratic_;

	bool is_shadow_casting_;
public:
	explicit LightNode(const std::string& name);
	~LightNode();

	virtual void set_color(const glm::vec3 diffuse, const glm::vec3 specular);
	virtual void set_params(const float linear, const float quadratic);

	std::vector<RenderingNode*> get_rendering_nodes() override;
	std::vector<LightNode*> get_light_nodes() override;

	ShaderResource* get_shader() const override
	{
		return nullptr;
	}

	virtual int get_light_type() = 0;

	glm::vec3 get_diffuse() const
	{
		return diffuse_;
	}

	glm::vec3 get_specular() const
	{
		return specular_;
	}

	float get_linear() const
	{
		return linear_;
	}

	float get_quadratic() const
	{
		return quadratic_;
	}

	glm::vec3 get_direction() const
	{
		// TODO
		return glm::vec3();
	}
};

