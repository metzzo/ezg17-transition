#pragma once
#include "RenderingNode.h"
#include "MainShader.h"

class CameraNode :
	public RenderingNode
{
	int program_id_;
public:
	CameraNode(const string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	void before_render() const override;

	void set_model_matrix(const glm::mat4& trafo) override;
};

