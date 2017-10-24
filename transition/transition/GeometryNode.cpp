#include "GeometryNode.h"
#include "MeshResource.h"
#include "RenderingNode.h"
#include "MainShader.h"

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

void GeometryNode::draw(RenderingNode *rendering_node) const
{
	const auto trafo = this->get_transformation();
	//TODO: I admit, this is very ugly, but it's a quick fix. Make this more beautiful
	MainShader* shader = dynamic_cast<MainShader*>(rendering_node->getShader());
	shader->set_diffuse_texture(resource_->get_material().get_texture());
	shader->set_model(this->get_transformation());

	glBindVertexArray(this->resource_->get_resource_id());
	glDrawElements(GL_TRIANGLES, this->resource_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void GeometryNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);
}