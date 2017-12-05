#include "LightNode.h"
#include "ILightShader.h"
#include "RenderingEngine.h"
#include "DepthOnlyShader.h"

LightNode::LightNode(const std::string& name, const LightType light_type): RenderingNode(name, glm::ivec2(),
                                                                                         glm::mat4())
{
	this->linear_ = 0;
	this->quadratic_ = 0;
	this->constant_ = 0;
	this->is_shadow_casting_ = false;
	this->shadow_map_size_ = 0;
	this->light_type_ = light_type;

	this->depth_map_fbo_ = -1;
	this->depth_map_ = -1;
}

LightNode::~LightNode()
{
}

void LightNode::set_color(const glm::vec3 diffuse, const glm::vec3 specular)
{
	this->diffuse_ = diffuse;
	this->specular_ = specular;
}

void LightNode::set_attenuation(const float constant, const float linear, const float quadratic)
{
	this->constant_ = constant;
	this->linear_ = linear;
	this->quadratic_ = quadratic;
}

void LightNode::set_shadow_casting(const bool is_shadow_casting, const int shadow_map_size)
{
	this->is_shadow_casting_ = is_shadow_casting;
	this->shadow_map_size_ = shadow_map_size;

	this->viewport_ = glm::ivec2(this->shadow_map_size_, this->shadow_map_size_);

	// TODO: set projection matrix
}

std::vector<RenderingNode*> LightNode::get_rendering_nodes()
{
	if (this->is_shadow_casting_)
	{
		return{ this };
	} else
	{
		return std::vector<RenderingNode*>();
	}
}

std::vector<LightNode*> LightNode::get_light_nodes()
{
	return{ this };
}

void LightNode::init(RenderingEngine* rendering_engine)
{
	RenderingNode::init(rendering_engine);

	if (this->is_shadow_casting_) {
		glGenFramebuffers(1, &this->depth_map_fbo_);
		glGenTextures(1, &this->depth_map_);

		glBindTexture(GL_TEXTURE_2D, this->depth_map_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			this->shadow_map_size_, this->shadow_map_size_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, this->depth_map_fbo_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth_map_, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void LightNode::before_render(const std::vector<LightNode*>& light_nodes) const
{
	RenderingNode::before_render(light_nodes);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depth_map_fbo_);
	glClear(GL_DEPTH_BUFFER_BIT);

	const auto shader = this->get_shader();
	shader->use();
	shader->set_camera_uniforms(this);
}

void LightNode::after_render() const
{
	RenderingNode::after_render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShaderResource* LightNode::get_shader() const
{
	return this->get_rendering_engine()->get_depth_only_shader();
}

void LightNode::set_transformation(const glm::mat4& trafo, const glm::mat4& itrafo)
{
	TransformationNode::set_transformation(trafo, itrafo);

	this->direction_ = glm::transpose(itrafo)*glm::vec4(-1, 0, 0, 1);
}

void LightNode::set_transformation(const glm::mat4& trafo)
{
	this->set_transformation(trafo, glm::inverse(trafo));
}

void LightNode::apply_transformation(const glm::mat4& trafo, const glm::mat4& itrafo)
{
	TransformationNode::apply_transformation(trafo, itrafo);

	this->direction_ = glm::transpose(itrafo)*glm::vec4(-1, 0, 0, 1);
}
