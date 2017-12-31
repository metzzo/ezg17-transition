#include "BloomEffect.h"

BloomEffect::BloomEffect(unsigned int intensity)
{
	this->intensity_ = intensity;
}

void BloomEffect::init(RenderingEngine *engine, CameraNode *camera)
{
	screenMesh_ = MeshResource::create_sprite(nullptr);
	screenMesh_->init();
	viewport_ = engine->get_viewport();
	gauss_shader_ = new BloomGaussShader();
	add_shader_ = new BloomAddShader();
	engine->register_resource(gauss_shader_);
	engine->register_resource(add_shader_);
	gauss_shader_->init();
	add_shader_->init();
	help_buffer_[0] = new TextureFBO(engine->get_viewport().x, engine->get_viewport().y, 1);
	help_buffer_[1] = new TextureFBO(engine->get_viewport().x, engine->get_viewport().y, 1);
}

void BloomEffect::perform_effect(const TextureFBO * from, GLuint fbo_to)
{
	TextureRenderable * brighttex = from->get_texture(1);
	gauss_shader_->use();
	gauss_shader_->set_gauss_uniforms(brighttex, true);

	bool horizontal = true;
	for (unsigned int i = 0; i < 2 * intensity_; i++) {
		help_buffer_[horizontal]->bind_for_rendering();
		if (i != 0) {
			gauss_shader_->set_gauss_uniforms(help_buffer_[!horizontal], horizontal);
		}
		glViewport(0, 0, viewport_.x, viewport_.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(screenMesh_->get_resource_id());
		glDrawElements(GL_TRIANGLES, screenMesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		horizontal = !horizontal;
	}

	add_shader_->use();
	add_shader_->set_textures(from->get_texture(0), help_buffer_[!horizontal]);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_to);
	glViewport(0, 0, viewport_.x, viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screenMesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screenMesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}
