#include "CameraNode.h"
#include "RenderingEngine.h"
#include "MainShader.h"
#include "LightNode.h"
#include "VolumetricLightingShader.h"

CameraNode::CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection) : RenderingNode(name, viewport, projection)
{
	screen_mesh_ = nullptr;
	render_target1_ = nullptr;
	render_target2_ = nullptr;
	volumetric_lighting_shader_ = nullptr;
	volumetric_buffer_ = nullptr;
}

CameraNode::~CameraNode()
{
	for (auto ef = effects_.begin(); ef != effects_.end(); ef++) {
		delete (*ef);
	}
}

void CameraNode::init(RenderingEngine *rendering_engine)
{
	RenderingNode::init(rendering_engine);

	screen_mesh_ = MeshResource::create_sprite(nullptr);
	render_target1_ = new TextureFBO(rendering_engine->get_viewport().x, rendering_engine->get_viewport().y, 2);
	render_target2_ = new TextureFBO(rendering_engine->get_viewport().x, rendering_engine->get_viewport().y, 2);
	volumetric_buffer_ = new TextureFBO(rendering_engine->get_viewport().x / 2, rendering_engine->get_viewport().y / 2, 1);
	volumetric_lighting_shader_ = new VolumetricLightingShader();
	volumetric_lighting_shader_->init();
	rendering_engine->register_resource(volumetric_lighting_shader_);

	for (auto &eff : effects_)
	{
		eff->init(rendering_engine, this);
	}
}

void CameraNode::add_post_processing_effect(PostProcessingEffect * effect)
{
	effects_.push_back(effect);
}

void CameraNode::before_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const
{
	for (auto &light : light_nodes)
	{
		light->render(drawables, std::vector<LightNode*>());
	}

	// render volumetric lighting volume
	volumetric_buffer_->bind_for_rendering();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, this->viewport_.x/2, this->viewport_.y/2);
	volumetric_lighting_shader_->use();
	volumetric_lighting_shader_->set_seed(glfwGetTime() * 10000);
	volumetric_lighting_shader_->set_light_uniforms(light_nodes);
	volumetric_lighting_shader_->set_camera_uniforms(this);

	for (auto &drawable : drawables)
	{
		drawable->draw(volumetric_lighting_shader_);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	RenderingNode::before_render(drawables, light_nodes);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto shader = this->get_shader();
	shader->use();
	shader->set_light_uniforms(light_nodes);
	shader->set_camera_uniforms(this);
	
	if (effects_.size() == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		render_target1_->bind_for_rendering();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CameraNode::after_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const
{
	RenderingNode::after_render(drawables, light_nodes);

	for (int i = 0; i < effects_.size(); i++) {
		auto ef = effects_.at(i);
		if (i%2 == 0) {
			ef->perform_effect(render_target1_, (i == effects_.size()-1) ? 0 : render_target2_->get_fbo_id());
		}
		else {
			ef->perform_effect(render_target2_, (i == effects_.size() - 1) ? 0 : render_target1_->get_fbo_id());
		}
	}
}

MainShader* CameraNode::get_shader() const 
{
	return this->get_rendering_engine()->get_main_shader();
}