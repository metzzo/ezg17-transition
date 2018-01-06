#include "RenderingNode.h"
#include "glheaders.h"
#include "IDrawable.h"
#include "ParticleEmitterNode.h"

RenderingNode::RenderingNode(const std::string& name, const glm::ivec2 viewport, const float fieldOfView, const float ratio, const float nearp, const float farp, const bool culling) : TransformationNode(name)
{
	this->viewport_ = viewport;
	this->set_projection_matrix(glm::perspective(glm::radians(fieldOfView), ratio, nearp, farp));
	this->culling_ = culling;
	if (culling_) {
		frustum_ = new FrustumG();
		frustum_->setCamInternals(glm::radians(fieldOfView), ratio, nearp, farp);
	}
}

RenderingNode::RenderingNode(const std::string & name, const glm::ivec2 viewport, const glm::mat4 & proj) : TransformationNode(name)
{
	this->viewport_ = viewport;
	this->set_projection_matrix(proj);
	this->frustum_ = nullptr;
	this->culling_ = false;
}

RenderingNode::~RenderingNode()
{
	if (frustum_ != nullptr) {
		delete frustum_;
	}
}

void RenderingNode::before_render(const std::vector<IDrawable*> &drawables, const std::vector<IDrawable*>& transparents, const std::vector<LightNode*> &light_nodes) const
{
	if (culling_) {
		glm::vec3 position = this->get_position();
		glm::vec3 direction = glm::inverse(glm::transpose(get_transformation())) * glm::vec4(0, 0, -1, 0);
		frustum_->setCamDef(position, position + direction, glm::vec3(0, 1, 0));
	}
	glViewport(0, 0, this->viewport_.x, this->viewport_.y);
}

void RenderingNode::set_viewport(const glm::ivec2 viewport)
{
	this->viewport_ = viewport;
}

const FrustumG* RenderingNode::get_frustum() const
{
	return this->frustum_;
}

void RenderingNode::initialize_culling(const float fieldOfView, const float ratio, const float nearp, const float farp)
{
	if (frustum_ != nullptr)
	{
		delete frustum_;
	}
	frustum_ = new FrustumG();
	frustum_->setCamInternals(glm::radians(fieldOfView), ratio, nearp, farp);
	culling_ = true;
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
		if (drawable->is_enabled()) {
			bool drawing = true;
			if (culling_) {
				int res = frustum_->sphereInFrustum(drawable->get_position(), drawable->get_bounding_sphere_radius());
				drawing = (res != FrustumG::OUTSIDE);
			}
			if (drawing) {
				drawable->draw(this->get_shader());
			}
		}
	}

	for (auto &transparent : transparents)
	{
		if (transparent->is_enabled()) {
			bool drawing = true;
			if (culling_) {
				int res = frustum_->sphereInFrustum(transparent->get_position(), transparent->get_bounding_sphere_radius());
				drawing = (res != FrustumG::OUTSIDE);
			}
			if (drawing) {
				transparent->draw(this->get_shader());
			}
		}
	}

	if (this->renders_particles()) {
		for (auto& emitter : emitters) {
			if (emitter->is_enabled()) {
				emitter->draw_particles(this);
			}
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
