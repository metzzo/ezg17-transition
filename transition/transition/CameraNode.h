#pragma once
#include "RenderingNode.h"
#include "MainShader.h"
#include "PostProcessingEffect.h"
#include "DummyShader.h"

class VolumetricLightingEffect;
class MeshResource;
class VolumetricLightingShader;
class TextureFBO;
class BloomEffect;
class MeshResource;
class DummyShader;


class CameraNode :
	public RenderingNode
{

private:
	TextureFBO *volumetric_lighting_result_render_target_;
	TextureFBO *main_render_target_;
	VolumetricLightingEffect *volumetric_lighting_effect_;
	BloomEffect *bloom_effect_;
	TextureResource* credits_texture_;
public:
	CameraNode(const std::string& name, const glm::ivec2& viewport, const float fieldOfView, const float ratio, const float nearp, const float farp, const bool culling);
	~CameraNode();

	
	virtual void init(RenderingEngine *rendering_engine) override;

	void before_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const override;
	void after_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const override;

	MainShader* get_shader() const override;

	bool renders_particles() const override { return true; }
	
	void set_bloom_params(int iterations, float treshold, float addintensity);

	void set_end_tex_intensity(float end_tex_intensity);
};

