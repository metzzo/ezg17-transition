#pragma once
#include "RenderingNode.h"
#include "MainShader.h"
#include "PostProcessingEffect.h"
#include "MeshResource.h"

class VolumetricLightingEffect;
class MeshResource;
class VolumetricLightingShader;
class TextureFBO;
class BloomEffect;


class CameraNode :
	public RenderingNode
{

private:
	MeshResource *screen_mesh_;
	TextureFBO *volumetric_lighting_result_render_target_;
	TextureFBO *main_render_target_;
	VolumetricLightingEffect *volumetric_lighting_effect_;
	BloomEffect *bloom_effect_;

public:
	CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	
	virtual void init(RenderingEngine *rendering_engine) override;

	void before_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;
	void after_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;

	MainShader* get_shader() const override;

	bool renders_particles() const override { return true; }
};

