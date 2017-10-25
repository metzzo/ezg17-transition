#include "GeometryNode.h"

GeometryNode::GeometryNode(const std::string& name, const MeshResource *resource) : TransformationNode(name)
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

void GeometryNode::draw(ShaderResource *shader) const
{
	const auto trafo = this->get_transformation();
	shader->set_model_uniforms(this);

	glBindVertexArray(this->resource_->get_resource_id());
	glDrawElements(GL_TRIANGLES, this->resource_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void GeometryNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);
}

const MeshResource* GeometryNode::get_mesh_resource() const {
	return resource_;
}