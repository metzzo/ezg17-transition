#pragma once
#include "Node.h"
#include "IDrawable.h"

class MeshResource;

class GeometryNode :
	public Node, IDrawable
{
	const MeshResource* resource_;
public:
	explicit GeometryNode(const string& name, const MeshResource *resource);
	~GeometryNode();

	std::vector<IDrawable*> get_drawables() override;

	void draw() const override;
	void init(RenderingEngine* rendering_engine) override;
};

