#pragma once
#include "Node.h"
#include <vector>
#include <glm/glm.hpp>
#include "IDrawable.h"

class RenderingNode :
	public Node, 
	public ITransformable
{
protected:
	glm::ivec2 viewport_;
	glm::mat4 projection_;
public:
	RenderingNode(const std::string& name, const glm::ivec2 viewport, const glm::mat4 projection);
	~RenderingNode();

	std::vector<RenderingNode*> get_rendering_nodes() override;

	virtual void before_render(const std::vector<LightNode*> &light_nodes) const;
	virtual void after_render() const;
	void render(const std::vector<IDrawable*> &drawables, const std::vector<LightNode*> &light_nodes);

	virtual void set_model_matrix(const glm::mat4& trafo) = 0;
};

