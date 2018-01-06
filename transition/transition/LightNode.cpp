#include "LightNode.h"
#include "ILightShader.h"
#include "RenderingEngine.h"
#include "DirectionalDepthShader.h"
#include "DirectionalShadowStrategy.h"

LightNode::LightNode(const std::string& name, const LightType light_type) : RenderingNode(
	name, 
	glm::ivec2(),
	glm::mat4())
{
	this->linear_ = 0;
	this->quadratic_ = 0;
	this->constant_ = 0;
	this->shadow_strategy_ = nullptr;
	this->light_type_ = light_type;
	this->cutoff_ = 0.0f;
	this->outer_cutoff_ = 0.0f;
	this->volumetric_ = false;
	this->phi_ = 0.0;
	this->tau_ = 0.0;
	this->min_bias_ = 0.0;
	this->max_bias_ = 0.0;
	this->has_fog_ = false;
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

void LightNode::set_shadow_strategy(IShadowStrategy* shadow_strategy, float min_bias, float max_bias)
{
	this->shadow_strategy_ = shadow_strategy;
	this->min_bias_ = min_bias;
	this->max_bias_ = max_bias;
}

void LightNode::set_cutoff(const float cutoff, const float outer_cutoff)
{
	this->cutoff_ = cutoff;
	this->outer_cutoff_ = outer_cutoff;
}

std::vector<LightNode*> LightNode::get_light_nodes()
{
	return{ this };
}

void LightNode::init(RenderingEngine* rendering_engine)
{
	RenderingNode::init(rendering_engine);

	if (this->shadow_strategy_) {
		this->shadow_strategy_->init(this);
	}
}

void LightNode::before_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
	RenderingNode::before_render(drawables, transparents, light_nodes);

	if (this->shadow_strategy_)
	{
		this->shadow_strategy_->before_render(this);
	}
}

void LightNode::after_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
	if (this->shadow_strategy_)
	{
		this->shadow_strategy_->after_render(this);
	}
	RenderingNode::after_render(drawables, transparents, light_nodes);
}

bool LightNode::is_rendering_enabled() const
{
	return this->is_enabled() && this->shadow_strategy_ != nullptr;
}

ShaderResource* LightNode::get_shader() const
{
	return this->shadow_strategy_ != nullptr ? this->shadow_strategy_->get_shader(this) : nullptr;
}

void LightNode::set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo)
{
	TransformationNode::set_transformation(trafo, itrafo);

	this->direction_ = glm::transpose(itrafo) * glm::vec4(0, 0, -1, 0);
}

void LightNode::set_transformation(const glm::mat4& trafo)
{
	this->set_transformation(trafo, glm::inverse(trafo));
}

void LightNode::apply_transformation(const glm::mat4& trafo, const glm::mat4& itrafo)
{
	TransformationNode::apply_transformation(trafo, itrafo);

	this->direction_ = glm::transpose(this->get_inverse_transformation()) * glm::vec4(direction_, 0.0f);
}

void LightNode::set_uniforms(ILightShader* shader)
{
	if (this->shadow_strategy_)
	{
		this->shadow_strategy_->set_uniforms(shader, this);
	}
}

/**
 * \brief 
 * \param is_volumetric is this light node actually volumetric
 * \param phi power of light source
 * \param tau probability of collision
 */
void LightNode::set_volumetric(const bool is_volumetric, float phi, float tau, bool has_fog)
{
	this->volumetric_ = is_volumetric;
	this->phi_ = phi;
	this->tau_ = tau;
	this->has_fog_ = has_fog;
}
