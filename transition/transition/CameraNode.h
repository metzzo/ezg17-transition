#pragma once
#include "RenderingNode.h"
#include "MainShader.h"

class CameraNode :
	public RenderingNode
{
public:
	CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	void before_render(const std::vector<LightNode*> &light_nodes) const override;

	void set_model_matrix(const glm::mat4& trafo) override;
};

