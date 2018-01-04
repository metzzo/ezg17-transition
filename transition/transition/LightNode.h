#pragma once
#include <glm/glm.hpp>
#include "RenderingNode.h"
#include "TextureResource.h"

class ILightShader;
class IShadowStrategy;

enum LightType
{
	DIRECTIONAL_LIGHT = 1,
	POINT_LIGHT = 2,
	SPOT_LIGHT = 3
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

	float cutoff_;
	float outer_cutoff_;

	LightType light_type_;

	glm::vec3 direction_;

	IShadowStrategy *shadow_strategy_;
	float min_bias_;
	float max_bias_;

	// volumetric parameters
	bool volumetric_; 
	float phi_; // power of light source
	float tau_; // probability of collision
public:
	explicit LightNode(const std::string& name, LightType light_type);
	~LightNode();

	void set_color(const glm::vec3 diffuse, const glm::vec3 specular);
	void set_attenuation(const float constant, const float linear, const float quadratic);
	void set_shadow_strategy(IShadowStrategy *shadow_strategy, float min_bias = 0.0001, float max_bias = 0.001);
	void set_cutoff(const float cutoff, const float outer_cutoff);

	std::vector<LightNode*> get_light_nodes() override;

	void init(RenderingEngine* rendering_engine) override;

	void before_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodess) const override;
	void after_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;
	bool is_rendering_enabled() const override;

	void set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo) override;
	void set_transformation(const glm::mat4& trafo) override;
	void apply_transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation) override;

	void set_uniforms(ILightShader *shader);
	void set_volumetric(const bool is_volumetric, float phi, float tau);

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

	float get_cutoff() const
	{
		return this->cutoff_;
	}

	float get_outer_cutoff() const
	{
		return this->outer_cutoff_;
	}
	IShadowStrategy *get_shadow_strategy() const
	{
		return this->shadow_strategy_;
	}

	bool is_volumetric() const
	{
		return this->volumetric_;
	}

	float get_phi() const
	{
		return this->phi_;
	}
	
	float get_tau()  const
	{
		return this->tau_;
	}

	float get_min_bias() const
	{
		return this->min_bias_;
	}

	float get_max_bias() const
	{
		return this->max_bias_;
	}
};

class IShadowStrategy
{
public:
	virtual ~IShadowStrategy() = default;
	virtual void init(LightNode *light_node) = 0;

	virtual void before_render(const LightNode *light_node) = 0;
	virtual void after_render(const LightNode *light_node) = 0;
	virtual ShaderResource *get_shader(const LightNode *light_node) = 0;
	virtual void set_uniforms(ILightShader* shader, LightNode* light_node) = 0;
};

