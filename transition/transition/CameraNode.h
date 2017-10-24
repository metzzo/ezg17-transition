#pragma once
#include "RenderingNode.h"
#include "MainShader.h"

class CameraNode :
	public RenderingNode
{
public:
	CameraNode(const std::string& name, const glm::ivec2& viewport, const glm::mat4& projection);
	~CameraNode();

	void before_render() const override;

	ShaderResource* getShader() const override;
};

