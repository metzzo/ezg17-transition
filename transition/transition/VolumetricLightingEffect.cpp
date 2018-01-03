#include "VolumetricLightingEffect.h"
#include "VolumetricLightingBlurShader.h"
#include "CameraNode.h"
#include "VolumetricLightingUpSampleShader.h"

VolumetricLightingEffect::VolumetricLightingEffect()
{
	this->screen_mesh_ = nullptr;
	this->blur_shader_ = nullptr;
	this->upsample_shader_ = nullptr;
	this->vert_blur_fbo_ = nullptr;
	this->hori_blur_fbo_ = nullptr;
	this->volumetric_buffer_ = nullptr;
}

VolumetricLightingEffect::~VolumetricLightingEffect()
{
	delete this->vert_blur_fbo_;
	delete this->hori_blur_fbo_;
}

void VolumetricLightingEffect::init(RenderingEngine* engine, CameraNode *camera)
{
	screen_mesh_ = MeshResource::create_sprite(nullptr);
	screen_mesh_->init();
	viewport_ = engine->get_viewport();
	blur_shader_ = new VolumetricLightingBlurShader();
	engine->register_resource(blur_shader_);
	blur_shader_->init();
	volumetric_buffer_ = camera->get_volumetric_buffer();

	vert_blur_fbo_ = new TextureFBO(viewport_.x / 2, viewport_.y / 2, 1);
	hori_blur_fbo_ = new TextureFBO(viewport_.x / 2, viewport_.y / 2, 1);
	
	upsample_shader_ = new VolumetricLightingUpSampleShader();
	engine->register_resource(upsample_shader_);
	upsample_shader_->init();

}

void VolumetricLightingEffect::perform_effect(const TextureFBO* from, GLuint fbo_to)
{
	TextureRenderable *scene_tex = from->get_texture(0);
	blur_shader_->use();
	blur_shader_->set_volumetric_texture(volumetric_buffer_);
	blur_shader_->set_vertical_pass(true);

	const glm::ivec2 size = this->vert_blur_fbo_->get_size();
	glBindFramebuffer(GL_FRAMEBUFFER, this->vert_blur_fbo_->get_fbo_id());
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	blur_shader_->set_vertical_pass(false);
	blur_shader_->set_volumetric_texture(vert_blur_fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, this->hori_blur_fbo_->get_fbo_id());
	glViewport(0, 0, size.x, size.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	upsample_shader_->use();
	upsample_shader_->set_volumetric_texture(hori_blur_fbo_);
	upsample_shader_->set_scene_texture(scene_tex);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_to);
	glViewport(0, 0, viewport_.x, viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

