#pragma once
#include "RenderingNode.h"
#include "MainShader.h"
#include "PostProcessingEffect.h"
#include "MeshResource.h"
#include "TextureFBO.h"

class VolumetricLightingShader;

class CameraNode :
	public RenderingNode
{

private:
	std::vector<PostProcessingEffect*> effects_;
	MeshResource *screen_mesh_;
	TextureFBO *render_target1_;
	TextureFBO *render_target2_;
	TextureFBO *volumetric_buffer_;
	VolumetricLightingShader *volumetric_lighting_shader_;
public:
	CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	
	virtual void init(RenderingEngine *rendering_engine) override;

	void add_post_processing_effect(PostProcessingEffect* effect);
	void before_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;
	void after_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;

	MainShader *get_shader() const override;

	TextureFBO *get_volumetric_buffer() const
	{
		return this->volumetric_buffer_;
	}
};

