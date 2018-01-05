#include "CameraNode.h"
#include "RenderingEngine.h"
#include "MainShader.h"
#include "LightNode.h"
#include "VolumetricLightingShader.h"
#include "VolumetricLightingEffect.h"
#include "BloomEffect.h"

CameraNode::CameraNode(const std::string& name, const glm::ivec2& viewport, const float fieldOfView, const float ratio, const float nearp, const float farp, const bool culling) : RenderingNode(name, viewport, fieldOfView, ratio, nearp, farp, culling)
{
	screen_mesh_ = nullptr;
	volumetric_lighting_result_render_target_ = nullptr;
	main_render_target_ = nullptr;

	volumetric_lighting_effect_ = new VolumetricLightingEffect();
	bloom_effect_ = new BloomEffect(1);
}

CameraNode::~CameraNode()
{
	delete volumetric_lighting_effect_;
	delete bloom_effect_;
}

void CameraNode::init(RenderingEngine *rendering_engine)
{
	RenderingNode::init(rendering_engine);

	screen_mesh_ = MeshResource::create_sprite(nullptr);

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
		light->render(drawables, {}, {}, std::vector<LightNode*>());
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