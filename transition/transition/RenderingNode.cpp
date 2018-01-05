#include "RenderingNode.h"
#include "glheaders.h"
#include "IDrawable.h"
#include "ParticleEmitterNode.h"

RenderingNode::RenderingNode(const std::string& name, const glm::ivec2 viewport, const glm::mat4 projection) : TransformationNode(name)
{
	this->viewport_ = viewport;
	this->set_projection_matrix(projection);
}

RenderingNode::~RenderingNode()
{
}

void RenderingNode::before_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
	glViewport(0, 0, this->viewport_.x, this->viewport_.y);
}

void RenderingNode::set_viewport(const glm::ivec2 viewport)
{
	this->viewport_ = viewport;
}

void RenderingNode::after_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
}

void RenderingNode::render(const std::vector<IDrawable*>& drawables, const std::vector<IDrawable*>& transparents, const std::vector<ParticleEmitterNode*> &emitters, const std::vector<LightNode*>& light_nodes) const
{
	if (!this->is_rendering_enabled())
	{
		return;
	}

	before_render(drawables, transparents, light_nodes);
	
	for (auto &drawable : drawables)
	{
		drawable->draw(this->get_shader());
	}

	for (auto &transparent : transparents)
	{
		transparent->draw(this->get_shader());
	}

	if (this->renders_particles()) {
		for (auto& emitter : emitters) {
			emitter->draw_particles(this);
		}
	}

	after_render(drawables, transparents, light_nodes);
}

bool RenderingNode::is_rendering_enabled() const
{
	return true;
}

const glm::mat4& RenderingNode::get_projection_matrix() const {
	return projection_;
}

const glm::mat4& RenderingNode::get_projection_inverse_matrix() const
{
	return this->projection_inv_;
}

const glm::mat4& RenderingNode::get_view_matrix() const {
	return this->get_inverse_transformation();
}

void RenderingNode::set_view_matrix(const glm::mat4& mat) {
	this->set_transformation(glm::inverse(mat), mat);
}

void RenderingNode::set_projection_matrix(const glm::mat4& mat)
{
	this->projection_ = mat;
	this->projection_inv_ = glm::inverse(this->projection_);
}
