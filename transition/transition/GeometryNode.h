#pragma once
#include "Node.h"
#include "IDrawable.h"

class GeometryNode :
	public Node, IDrawable
{
public:
	GeometryNode();
	~GeometryNode();

	std::vector<IDrawable*> get_drawables() override;

	void draw() const override;
	void init(RenderingEngine* rendering_engine) override;
};

