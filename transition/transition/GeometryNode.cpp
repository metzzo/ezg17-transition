#include "GeometryNode.h"
#include "MeshResource.h"
#include "RenderingNode.h"
#include "ITransformable.h"

GeometryNode::GeometryNode(const string& name, const MeshResource *resource) : Node(name)
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

void GeometryNode::draw(RenderingNode *rendering_node) const
{
	const auto trafo = this->get_transformation();
	rendering_node->set_model_matrix(trafo);

	glBindVertexArray(this->resource_->get_resource_id());
	glDrawElements(GL_TRIANGLES, this->resource_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
}

void GeometryNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);
}
