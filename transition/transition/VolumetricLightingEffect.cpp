#include "VolumetricLightingEffect.h"
#include "VolumetricLightingBlurShader.h"
#include "CameraNode.h"

VolumetricLightingEffect::VolumetricLightingEffect()
{
	this->screen_mesh_ = nullptr;
	this->shader_ = nullptr;
}

void VolumetricLightingEffect::init(RenderingEngine* engine, CameraNode *camera)
{
	screen_mesh_ = MeshResource::create_sprite(nullptr);
	screen_mesh_->init();
	viewport_ = engine->get_viewport();
	shader_ = new VolumetricLightingBlurShader();
	engine->register_resource(shader_);
	shader_->init();
	volumetric_buffer_ = camera->get_volumetric_buffer();
}

void VolumetricLightingEffect::perform_effect(const TextureFBO* from, GLuint fbo_to)
{
	TextureRenderable * scene_tex = from->get_texture(0);
	shader_->use();
	shader_->set_scene_texture(scene_tex);
	shader_->set_volumetric_texture(volumetric_buffer_);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_to);
	glViewport(0, 0, viewport_.x, viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screen_mesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screen_mesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}

