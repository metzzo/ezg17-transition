#include "DummyEffect.h"

DummyEffect::DummyEffect()
{
}

void DummyEffect::init(RenderingEngine * engine, CameraNode *camera)
{
	screenMesh_ = MeshResource::create_sprite(nullptr);
	screenMesh_->init();
	viewport_ = engine->get_viewport();
	shader_ = new DummyShader();
	engine->register_resource(shader_);
	shader_->init();
}


void DummyEffect::perform_effect(const TextureFBO * from, GLuint fbo_to, const std::vector<LightNode *> light_nodes)
{
	TextureRenderable * tex = from->get_texture(0);
	shader_->use();
	shader_->set_texture(tex);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_to);
	glViewport(0, 0, viewport_.x, viewport_.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(screenMesh_->get_resource_id());
	glDrawElements(GL_TRIANGLES, screenMesh_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}
