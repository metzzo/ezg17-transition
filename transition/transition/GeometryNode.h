#pragma once
#include "TransformationNode.h"
#include "IDrawable.h"
#include "MeshResource.h"
#include "ShaderResource.h"

class MeshResource;

class GeometryNode :
	public TransformationNode, 
	public IDrawable
{
	MeshResource* resource_;
	float bounding_sphere_radius_ = 0;

public:
	explicit GeometryNode(const std::string& name, MeshResource *resource);
	~GeometryNode();

	std::vector<IDrawable*> get_drawables() override;
	std::vector<IDrawable*> get_transparent_drawables() override;

	void draw(ShaderResource *shader) const override;
	void init(RenderingEngine* rendering_engine) override;

	const MeshResource* get_mesh_resource() const;
	MeshResource* get_editable_mesh_resource();

	float get_bounding_sphere_radius() const override;

	glm::vec3 get_position() const override {
		return TransformationNode::get_position();
	}
};

