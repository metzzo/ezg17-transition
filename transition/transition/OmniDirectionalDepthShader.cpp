#include <cassert>
#include "LightNode.h"
#include "GeometryNode.h"
#include "OmniDirectionalDepthShader.h"
#include "OmniDirectionalShadowStrategy.h"


OmniDirectionalDepthShader::OmniDirectionalDepthShader() : ShaderResource("assets/shaders/depth_shader_omni_directional.vs", "assets/shaders/depth_shader_omni_directional.fs", "assets/shaders/depth_shader_omni_directional.gs")
{
	this->model_uniform_ = -1;
	this->light_pos_uniform_ = -1;
	this->far_plane_uniform_ = -1;
	for (auto i = 0; i < 6; i++)
	{
		this->shadow_transform_uniform_[i] = -1;
	}
}

OmniDirectionalDepthShader::~OmniDirectionalDepthShader()
{
}

void OmniDirectionalDepthShader::init()
{
	ShaderResource::init();

	this->model_uniform_ = get_uniform("model");
	this->light_pos_uniform_ = get_uniform("light_pos");
	this->far_plane_uniform_ = get_uniform("far_plane");

	for (auto i = 0; i < 6; i++)
	{
		this->shadow_transform_uniform_[i] = get_uniform("shadow_transform", i);
	}
}

void OmniDirectionalDepthShader::set_camera_uniforms(const RenderingNode* rendering_node)
{
	const auto &node = (LightNode*)rendering_node;

	assert(this->model_uniform_ >= 0);
	assert(this->light_pos_uniform_ >= 0);
	assert(this->far_plane_uniform_ >= 0);

	glm::mat4 shadow_transforms[6];
	const auto& light_pos = node->get_position();

	shadow_transforms[0] = glm::lookAt(light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	shadow_transforms[1] = glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	shadow_transforms[2] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shadow_transforms[3] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	shadow_transforms[4] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	shadow_transforms[5] = glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	for (auto i = 0; i < 6; i++)
	{
		assert(this->shadow_transform_uniform_[i] >= 0);

		const auto &transform = node->get_projection_matrix() * shadow_transforms[i];
		glUniformMatrix4fv(this->shadow_transform_uniform_[i], 1, GL_FALSE, &transform[0][0]);
	}

	// static cast is okay, since it only makes sense to use OmniDirectionalDepthShader with OmniDirectionalShadowStrategy
	const auto strategy = static_cast<OmniDirectionalShadowStrategy*>(node->get_shadow_strategy());
	assert(strategy != nullptr);
	glUniform1f(this->far_plane_uniform_, strategy->get_far_plane());
	glUniform3fv(this->light_pos_uniform_, 1, &node->get_position()[0]);
}

void OmniDirectionalDepthShader::set_model_uniforms(const GeometryNode* node)
{
	assert(this->model_uniform_ >= 0);
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
}
