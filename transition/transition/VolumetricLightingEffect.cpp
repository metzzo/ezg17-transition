#include "VolumetricLightingEffect.h"
#include "VolumetricLightingBlurShader.h"
#include "CameraNode.h"
#include "VolumetricLightingUpSampleShader.h"
#include "VolumetricLightingDownSampleShader.h"
#include "VolumetricLightingShader.h"
#include "DummyShader.h"

VolumetricLightingEffect::VolumetricLightingEffect()
{
	this->screen_mesh_ = nullptr;
	this->blur_shader_ = nullptr;
	this->upsample_shader_ = nullptr;
	this->ping_half_res_fbo_ = nullptr;
	this->pong_half_res_fbo_ = nullptr;
	this->depth_half_res_fbo_ = nullptr;
}

VolumetricLightingEffect::~VolumetricLightingEffect()
{
	delete this->ping_half_res_fbo_;
	delete this->pong_half_res_fbo_;
	delete this->depth_half_res_fbo_;
}

void VolumetricLightingEffect::init(RenderingEngine* engine, CameraNode *camera)
{
	this->screen_mesh_ = MeshResource::create_sprite(nullptr);
	this->screen_mesh_->init();

	this->camera_ = camera;
	this->viewport_ = engine->get_viewport();

	this->blur_shader_ = new VolumetricLightingBlurShader();
	engine->register_resource(this->blur_shader_);
	this->blur_shader_->init();

	this->ping_half_res_fbo_ = new TextureFBO(this->viewport_.x / 2, this->viewport_.y / 2, 1);
	this->ping_half_res_fbo_->init_color();

	this->pong_half_res_fbo_ = new TextureFBO(this->viewport_.x / 2, this->viewport_.y / 2, 1);
	this->pong_half_res_fbo_->init_color();

	this->depth_half_res_fbo_ = new TextureFBO(this->viewport_.x / 2, this->viewport_.y / 2, 1);
	this->depth_half_res_fbo_->init_depth();
	
	this->upsample_shader_ = new VolumetricLightingUpSampleShader();
	engine->register_resource(this->upsample_shader_);
	this->upsample_shader_->init();

	this->downsample_shader_ = new VolumetricLightingDownSampleShader();
	engine->register_resource(this->downsample_shader_);
	this->downsample_shader_->init();

	this->volumetric_lighting_shader_ = new VolumetricLightingShader();
	this->volumetric_lighting_shader_->init();
	engine->register_resource(this->volumetric_lighting_shader_);

	this->dummy_shader_ = new DummyShader();
	engine->register_resource(this->dummy_shader_);
	this->dummy_shader_->init();
}

void VolumetricLightingEffect::perform_effect(const TextureFBO* from, GLuint fbo_to, const std::vector<LightNode *> light_nodes)
{
	TextureRenderable *scene_tex = from->get_texture(0);
	TextureRenderable *depth_tex = from->get_texture(from->get_depth_index());
	const glm::ivec2 size = this->ping_half_res_fbo_->get_size();

	glDisable(GL_BLEND);

	// down sample scene using depth aware downsampling
	downsample_shader_->use();
	downsample_shader_->set_depth_texture(depth_tex);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_half_res_fbo_->get_fbo_id());
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// calculate volumetric lighting
	volumetric_lighting_shader_->use();
	volumetric_lighting_shader_->set_light_uniforms(light_nodes);
	volumetric_lighting_shader_->set_camera_uniforms(camera_);
	volumetric_lighting_shader_->set_depth_texture(depth_half_res_fbo_);

	glBindFramebuffer(GL_FRAMEBUFFER, pong_half_res_fbo_->get_fbo_id());
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);


	// blur volumetric lighting using depth aware gauss vertically
	blur_shader_->use();
	blur_shader_->set_volumetric_texture(pong_half_res_fbo_);
	blur_shader_->set_vertical_pass(true);

	glBindFramebuffer(GL_FRAMEBUFFER, this->ping_half_res_fbo_->get_fbo_id());
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// blur volumetric lighting using depth aware gauss horizontally
	blur_shader_->set_vertical_pass(false);
	blur_shader_->set_volumetric_texture(ping_half_res_fbo_);

	glBindFramebuffer(GL_FRAMEBUFFER, this->pong_half_res_fbo_->get_fbo_id());
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// get volumetric lighting to full resolution using depth aware upsampling
	upsample_shader_->use();
	upsample_shader_->set_volumetric_texture(pong_half_res_fbo_);
	upsample_shader_->set_scene_texture(scene_tex);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_to);
	glViewport(0, 0, viewport_.x, viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
}

