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
	glBindVertexArray(this->resource_->get_resource_id());
	glDrawElements(GL_TRIANGLES, this->resource_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
}

void GeometryNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);
}
