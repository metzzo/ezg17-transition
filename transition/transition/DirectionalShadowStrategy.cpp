#include "DirectionalShadowStrategy.h"
#include "RenderingNode.h"
#include "RenderingEngine.h"
#include "DirectionalDepthShader.h"
#include "ILightShader.h"
#include <glm/glm.hpp>

DirectionalShadowStrategy::DirectionalShadowStrategy(const int shadow_map_size, const float near_plane, const float far_plane)
{
	this->shadow_map_size_ = shadow_map_size;
	this->near_plane_ = near_plane;
	this->far_plane_ = far_plane;

	this->depth_map_ = -1;
	this->depth_map_fbo_ = -1;
}

DirectionalShadowStrategy::~DirectionalShadowStrategy()
{
}

void DirectionalShadowStrategy::init(LightNode* light_node)
{
	light_node->set_viewport(glm::ivec2(this->shadow_map_size_, this->shadow_map_size_));

	if (light_node->get_light_type() == SPOT_LIGHT)
	{
		light_node->set_projection_matrix(glm::perspective(glm::radians(2 * light_node->get_outer_cutoff()), 1.0f, this->near_plane_, this->far_plane_));
		light_node->initialize_culling(2 * light_node->get_outer_cutoff(), 1.0f, this->near_plane_, this->far_plane_);
	}
	else if (light_node->get_light_type() == DIRECTIONAL_LIGHT)
	{
		light_node->set_projection_matrix(glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, this->near_plane_, this->far_plane_));
	}

	glGenFramebuffers(1, &this->depth_map_fbo_);
	glGenTextures(1, &this->depth_map_);

	glBindTexture(GL_TEXTURE_2D, this->depth_map_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		this->shadow_map_size_, this->shadow_map_size_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depth_map_fbo_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth_map_, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowStrategy::before_render(const LightNode *light_node)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->depth_map_fbo_);
	glClear(GL_DEPTH_BUFFER_BIT);

	const auto shader = light_node->get_shader();
	shader->use();
	shader->set_camera_uniforms(light_node);

	if (!RE_CULLING) {
		glEnable(GL_CULL_FACE);
	}
	glCullFace(GL_FRONT);
}

void DirectionalShadowStrategy::after_render(const LightNode *light_node)
{
	if (!RE_CULLING) {
		glDisable(GL_CULL_FACE);
	}
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShaderResource* DirectionalShadowStrategy::get_shader(const LightNode *light_node)
{
	return light_node->get_rendering_engine()->get_directional_depth_shader();
}

void DirectionalShadowStrategy::set_uniforms(ILightShader* shader, LightNode* light_node)
{
	shader->set_directional_shadow_map_uniforms(light_node, this->depth_map_);
}
