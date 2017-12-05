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

	float constant_;
	float linear_;
	float quadratic_;

	LightType light_type_;

	bool is_shadow_casting_;
	int shadow_map_size_;
	GLuint depth_map_fbo_;
	GLuint depth_map_;

	glm::vec3 direction_;
public:
	explicit LightNode(const std::string& name, LightType light_type);
	~LightNode();

	void set_color(const glm::vec3 diffuse, const glm::vec3 specular);
	void set_attenuation(const float constant, const float linear, const float quadratic);
	void set_shadow_casting(bool is_shadow_casting, int shadow_map_size);

	std::vector<RenderingNode*> get_rendering_nodes() override;
	std::vector<LightNode*> get_light_nodes() override;

	void init(RenderingEngine* rendering_engine) override;
	void before_render(const std::vector<LightNode*>& light_nodes) const override;
	void after_render() const override;

	void set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo) override;
	void set_transformation(const glm::mat4& trafo) override;
	void apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation) override;

	ShaderResource* get_shader() const override;

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

	float get_constant() const
	{
		return constant_;
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
};

