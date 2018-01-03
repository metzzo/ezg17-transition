#pragma once
#include "PostProcessingEffect.h"
#include "VolumetricLightingUpSampleShader.h"
class MeshResource;
class VolumetricLightingBlurShader;

class VolumetricLightingEffect : public PostProcessingEffect {
private:
	MeshResource *screen_mesh_;
	VolumetricLightingBlurShader* blur_shader_;
	glm::ivec2 viewport_;
	TextureFBO* volumetric_buffer_;
	TextureFBO* vert_blur_fbo_;
	TextureFBO* hori_blur_fbo_;
	VolumetricLightingUpSampleShader* upsample_shader_;

public:
	VolumetricLightingEffect();
	virtual ~VolumetricLightingEffect();

	virtual void init(RenderingEngine *engine, CameraNode *camera) override;

	virtual void perform_effect(const TextureFBO *from, GLuint fbo_to) override;

};
