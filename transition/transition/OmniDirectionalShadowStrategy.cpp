#include "OmniDirectionalShadowStrategy.h"
#include "RenderingEngine.h"
#include "ShaderResource.h"
#include "OmniDirectionalDepthShader.h"
#include "ILightShader.h"

OmniDirectionalShadowStrategy::OmniDirectionalShadowStrategy(const int shadow_map_size, const float near_plane, const float far_plane)
{
	this->shadow_map_size_ = shadow_map_size;
	this->near_plane_ = near_plane;
	this->far_plane_ = far_plane;

	this->depth_cubemap_ = -1;
	this->depth_cubemap_fbo_ = -1;
}


OmniDirectionalShadowStrategy::~OmniDirectionalShadowStrategy()
{
}

void OmniDirectionalShadowStrategy::init(LightNode* light_node)
{
	light_node->set_viewport(glm::ivec2(this->shadow_map_size_, this->shadow_map_size_));

	light_node->set_projection_matrix(glm::perspective(glm::radians(90.0f), 1.0f, this->near_plane_, this->far_plane_));


	glGenFramebuffers(1, &this->depth_cubemap_fbo_);
	glGenTextures(1, &this->depth_cubemap_);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_cubemap_);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			shadow_map_size_, shadow_map_size_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depth_cubemap_fbo_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depth_cubemap_, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniDirectionalShadowStrategy::before_render(const LightNode *light_node)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->depth_cubemap_fbo_);
	glClear(GL_DEPTH_BUFFER_BIT);

	const auto shader = light_node->get_shader();
	shader->use();
	shader->set_camera_uniforms(light_node);

	glCullFace(GL_FRONT);
}

void OmniDirectionalShadowStrategy::after_render(const LightNode *light_node)
{
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShaderResource* OmniDirectionalShadowStrategy::get_shader(const LightNode *light_node)
{
	return light_node->get_rendering_engine()->get_omni_directional_depth_shader();
}

void OmniDirectionalShadowStrategy::set_uniforms(ILightShader* shader, LightNode* light_node)
{
	shader->set_omni_directional_shadow_map_uniforms(light_node, this->depth_cubemap_, this->far_plane_, this->near_plane_);
}
