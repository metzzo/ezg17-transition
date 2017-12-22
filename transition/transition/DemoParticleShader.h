#pragma once
#include "ShaderResource.h"
#include <glm/glm.hpp>

class DemoParticleShader : public ShaderResource {
private:
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
	
public:
	DemoParticleShader();
	~DemoParticleShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
	void set_modelmat_uniforms(const glm::mat4& trafo);
};