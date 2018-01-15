#pragma once
#include "PostProcessingEffect.h"
#include "BloomGaussShader.h"
#include "BloomAddShader.h"
#include "TextureFBO.h"

/*
Bloom-Effect
*/
class BloomEffect : public PostProcessingEffect {
private:
	MeshResource *screenMesh_;
	BloomGaussShader *gauss_shader_;
	BloomAddShader *add_shader_;
	TextureFBO *help_buffer_[2];	//Ping-Pong-Buffers
	unsigned int iterations_;
	float addintensity_  = 1.0f;
	glm::ivec2 viewport_;
	TextureRenderable *end_tex_; // texture that is displayed when reaching the end
	float end_tex_intensity_;
public:
	/*
	intensity: How strong the bloom should be. The number of 8x8-Gauss-Filter to use on the image.
	*/
	BloomEffect(unsigned int iterations, TextureRenderable *end_tex);

	virtual void init(RenderingEngine *engine, CameraNode *camera) override;

	virtual void perform_effect(const TextureFBO *from, GLuint fbo_to, const std::vector<LightNode *> light_nodes) override;

	void set_iterations(int iterations);
	void set_addintensity(float intensity);
	void set_end_tex_intensity(float end_tex_intensity);
};