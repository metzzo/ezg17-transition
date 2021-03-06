#include "CameraNode.h"
#include "RenderingEngine.h"
#include "MainShader.h"
#include "LightNode.h"
#include "VolumetricLightingShader.h"
#include "VolumetricLightingEffect.h"
#include "BloomEffect.h"
#include "DummyEffect.h"

CameraNode::CameraNode(const std::string& name, const glm::ivec2& viewport, const float fieldOfView, const float ratio, const float nearp, const float farp, const bool culling) : RenderingNode(name, viewport, fieldOfView, ratio, nearp, farp, culling)
{
	volumetric_lighting_result_render_target_ = nullptr;
	main_render_target_ = nullptr;

	volumetric_lighting_effect_ = new VolumetricLightingEffect();
	credits_texture_ = new TextureResource("assets/gfx/end.tga");
	bloom_effect_ = new BloomEffect(1, credits_texture_);
}

CameraNode::~CameraNode()
{
	delete volumetric_lighting_effect_;
	delete bloom_effect_;
}

void CameraNode::init(RenderingEngine *rendering_engine)
{
	RenderingNode::init(rendering_engine);

	credits_texture_->init();
	rendering_engine->register_resource(credits_texture_);

	main_render_target_ = new TextureFBO(rendering_engine->get_viewport().x, rendering_engine->get_viewport().y, 2);
	main_render_target_->init_color(true);

	volumetric_lighting_result_render_target_ = new TextureFBO(rendering_engine->get_viewport().x, rendering_engine->get_viewport().y, 2);
	volumetric_lighting_result_render_target_->init_color();

	volumetric_lighting_effect_->init(rendering_engine, this);
	bloom_effect_->init(rendering_engine, this);
}

void CameraNode::before_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
	for (auto &light : light_nodes)
	{
		light->render(drawables, transparents, {}, std::vector<LightNode*>());
	}

	RenderingNode::before_render(drawables, transparents, light_nodes);

	const auto shader = this->get_shader();
	shader->use();
	shader->set_light_uniforms(light_nodes);
	shader->set_camera_uniforms(this);
	
	main_render_target_->bind_for_rendering();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CameraNode::after_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
	RenderingNode::after_render(drawables, transparents, light_nodes);

	volumetric_lighting_effect_->perform_effect(main_render_target_, volumetric_lighting_result_render_target_->get_fbo_id(), light_nodes);
	bloom_effect_->perform_effect(volumetric_lighting_result_render_target_, 0, light_nodes);
}

MainShader* CameraNode::get_shader() const 
{
	return this->get_rendering_engine()->get_main_shader();
}

void CameraNode::set_bloom_params(int iterations, float treshold, float addintensity)
{
	bloom_effect_->set_iterations(iterations);
	bloom_effect_->set_addintensity(addintensity);
	volumetric_lighting_effect_->set_bloom_treshold(treshold);
}

void CameraNode::set_end_tex_intensity(float end_tex_intensity)
{
	this->bloom_effect_->set_end_tex_intensity(end_tex_intensity);
}
