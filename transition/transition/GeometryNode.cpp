#include "GeometryNode.h"
#include <iostream>

GeometryNode::GeometryNode(const std::string& name, MeshResource *resource) : TransformationNode(name)
{
	this->resource_ = resource;
}

GeometryNode::~GeometryNode()
{
}

std::vector<IDrawable*> GeometryNode::get_drawables()
{
	if (resource_->get_material().has_alpha_texture()) {
		return{};
	}
	else {
		return{ this };
	}
}

std::vector<IDrawable*> GeometryNode::get_transparent_drawables()
{
	if (resource_->get_material().has_alpha_texture()) {
		return{ this };
	}
	else {
		return{ };
	}
}

void GeometryNode::draw(ShaderResource *shader) const
{
	shader->set_model_uniforms(this);

	glBindVertexArray(this->resource_->get_resource_id());
	glDrawElements(GL_TRIANGLES, this->resource_->get_num_indices(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void GeometryNode::init(RenderingEngine* rendering_engine)
{
	Node::init(rendering_engine);

	//calculate bounding sphere radius
	bounding_sphere_radius_ = resource_->calculate_sphere_radius(this->get_transformation());
}

const MeshResource* GeometryNode::get_mesh_resource() const {
	return resource_;
}

MeshResource * GeometryNode::get_editable_mesh_resource()
{
	return resource_;
}

float GeometryNode::get_bounding_sphere_radius() const
{
	return bounding_sphere_radius_;
}
