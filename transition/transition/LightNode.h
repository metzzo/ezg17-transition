#pragma once
#include <glm/glm.hpp>
#include "RenderingNode.h"
class ILightShader;

enum LightType
{
	DIRECTIONAL_LIGHT = 1,
	POINT_LIGHT = 2
};

class LightNode :
	public RenderingNode
{
protected:
	glm::vec3 diffuse_;
	glm::vec3 specular_;
	glm::vec3 direction_;

	float linear_;
	float quadratic_;

	LightType light_type_;

	bool is_shadow_casting_;
public:
	explicit LightNode(const std::string& name, LightType light_type);
	~LightNode();

	void set_color(const glm::vec3 diffuse, const glm::vec3 specular);
	void set_params(const float linear, const float quadratic);

	std::vector<RenderingNode*> get_rendering_nodes() override;
	std::vector<LightNode*> get_light_nodes() override;

	ShaderResource* get_shader() const override
	{
		return nullptr;
	}

	LightType get_light_type() const
	{
		return light_type_;
	}

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
		return this->direction_;
	}

	void set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo) override;
	void apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation) override;
};

