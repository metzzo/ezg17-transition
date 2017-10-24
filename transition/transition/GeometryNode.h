#pragma once
#include "TransformationNode.h"
#include "IDrawable.h"

class MeshResource;

class GeometryNode :
	public TransformationNode, 
	public IDrawable
{
	const MeshResource* resource_;
public:
	explicit GeometryNode(const std::string& name, const MeshResource *resource);
	~GeometryNode();

	std::vector<IDrawable*> get_drawables() override;

	void draw(RenderingNode *rendering_node) const override;
	void init(RenderingEngine* rendering_engine) override;
};

