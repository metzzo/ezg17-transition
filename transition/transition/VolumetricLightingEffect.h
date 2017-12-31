#pragma once
#include "PostProcessingEffect.h"
class MeshResource;
class VolumetricLightingBlurShader;

class VolumetricLightingEffect : public PostProcessingEffect {
private:
	MeshResource *screen_mesh_;
	VolumetricLightingBlurShader* shader_;
	glm::ivec2 viewport_;
	TextureFBO* volumetric_buffer_;

public:
	VolumetricLightingEffect();

	virtual void init(RenderingEngine *engine, CameraNode *camera) override;

	virtual void perform_effect(const TextureFBO *from, GLuint fbo_to) override;

};
