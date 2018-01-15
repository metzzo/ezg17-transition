#pragma once
#include "ShaderResource.h"
#include <glm/glm.hpp>

class FootParticleShader : public ShaderResource {
private:
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
	GLint size_uniform_;
	GLint maxttl_uniform_;
	
public:
	FootParticleShader();
	~FootParticleShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
	void set_modelmat_uniforms(const glm::mat4& trafo, const glm::vec2& size, float max_ttl);
};