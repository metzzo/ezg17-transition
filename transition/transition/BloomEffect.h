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
	unsigned int intensity_;
	glm::ivec2 viewport_;

public:
	/*
	intensity: How strong the bloom should be. The number of 8x8-Gauss-Filter to use on the image.
	*/
	BloomEffect(unsigned int intensity);

	virtual void init(RenderingEngine *engine) override;

	virtual void perform_effect(const TextureFBO *from, GLuint fbo_to) override;

};