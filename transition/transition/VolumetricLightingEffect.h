#pragma once
#include "PostProcessingEffect.h"
class MeshResource;
class VolumetricLightingBlurShader;
class VolumetricLightingShader;
class VolumetricLightingUpSampleShader;
class VolumetricLightingDownSampleShader;
class DummyShader;

class VolumetricLightingEffect : public PostProcessingEffect {
	MeshResource *screen_mesh_;
	glm::ivec2 viewport_;

	VolumetricLightingUpSampleShader *upsample_shader_;
	VolumetricLightingDownSampleShader *downsample_shader_;
	VolumetricLightingShader *volumetric_lighting_shader_;
	VolumetricLightingBlurShader *blur_shader_;

	TextureFBO *pong_half_res_fbo_;
	TextureFBO *ping_half_res_fbo_;
	TextureFBO *depth_half_res_fbo_;

	CameraNode *camera_;
	DummyShader *dummy_shader_;
public:
	VolumetricLightingEffect();
	virtual ~VolumetricLightingEffect();

	virtual void init(RenderingEngine *engine, CameraNode *camera) override;

	virtual void perform_effect(const TextureFBO *from, GLuint fbo_to, const std::vector<LightNode *> light_nodes) override;

};
