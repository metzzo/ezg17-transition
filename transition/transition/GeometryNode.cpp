#include "GeometryNode.h"
#include "MeshResource.h"

GeometryNode::GeometryNode(const MeshResource *resource)
{
	this->resource_ = resource;
}

GeometryNode::~GeometryNode()
{
}

std::vector<IDrawable*> GeometryNode::get_drawables()
{
	return { this };
}

void GeometryNode::draw() const
{
	// TODO
}

void GeometryNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);
}
