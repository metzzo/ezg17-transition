#pragma once
#include "RenderingNode.h"
#include "MainShader.h"
#include "PostProcessingEffect.h"
#include "MeshResource.h"
#include "TextureFBO.h"

class CameraNode :
	public RenderingNode
{

private:
	std::vector<PostProcessingEffect*> effects_;
	MeshResource* screenMesh_;
	TextureFBO* renderTarget1_;
	TextureFBO* renderTarget2_;

public:
	CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	
	virtual void init(RenderingEngine *rendering_engine) override;

	void add_post_processing_effect(PostProcessingEffect* effect);
	void before_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;
	void after_render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const override;

	MainShader* get_shader() const override;

	bool renders_particles() const override { return true; }
};

