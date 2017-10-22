#pragma once
#include "Node.h"
#include <vector>
#include <glm/glm.hpp>
#include "IDrawable.h"

class RenderingNode :
	public Node, ITransformable
{
	glm::ivec2 viewport_;
	glm::mat4 projection_;
	glm::mat4 look_at_;
public:
	RenderingNode(const string& name, const glm::ivec2 viewport, const glm::mat4 projection);
	~RenderingNode();

	std::vector<RenderingNode*> get_rendering_nodes() override;

	virtual void before_render() const;
	virtual void after_render() const;
	void render(const std::vector<IDrawable*> &drawables) const;

	void set_look_at(const glm::mat4 look_at)
	{
		this->look_at_ = look_at;
	}

	const glm::mat4& get_look_at() const
	{
		return this->look_at_;
	}
};

