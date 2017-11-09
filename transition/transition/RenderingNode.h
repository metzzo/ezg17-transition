#pragma once
#include "TransformationNode.h"
#include <vector>
#include <glm/glm.hpp>
#include "IDrawable.h"
#include "ShaderResource.h"

class AnimatorNode;

class RenderingNode :
	public TransformationNode
{
protected:
	glm::ivec2 viewport_;
	glm::mat4 projection_;
	std::vector<AnimatorNode*> animators_;
public:
	RenderingNode(const std::string& name, const glm::ivec2 viewport, const glm::mat4 projection);
	~RenderingNode();

	std::vector<RenderingNode*> get_rendering_nodes() override;

	virtual void before_render(const std::vector<LightNode*> &light_nodes) const;
	virtual void after_render() const;
	void render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes) const;

	virtual ShaderResource* get_shader() const = 0;

	glm::mat4 get_projection_matrix() const;
	glm::mat4 get_view_matrix() const;
	void set_view_matrix(const glm::mat4& mat);
};

