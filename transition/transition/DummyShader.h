#pragma once
#include "ShaderResource.h"
#include "TextureResource.h"

/*
Dummy-PostProcessing-Shader for just displaying the given texture
*/
class DummyShader : public ShaderResource {

private:
	GLint texture_uniform_;

public:
	DummyShader();
	~DummyShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;

	void set_texture(TextureRenderable *tex);
};
