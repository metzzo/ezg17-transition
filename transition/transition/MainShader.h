#pragma once
#include "ShaderResource.h"
#include "ILightShader.h"
#include "GeometryNode.h"
#include "RenderingNode.h"

class LightNode;

const unsigned int max_nr_lights = 10;
class MainShader :
	public ShaderResource,
	public ILightShader
{
	GLint model_uniform_;
	GLint view_uniform_;
	GLint projection_uniform_;
	GLint diffuse_texture_uniform_;

	GLint num_lights_uniform_;
	GLint light_type_uniform_[max_nr_lights];
	GLint position_uniform_[max_nr_lights];
	GLint direction_uniform_[max_nr_lights];
	GLint linear_uniform_[max_nr_lights];
	GLint quadratic_uniform_[max_nr_lights];
	GLint diffuse_uniform_[max_nr_lights];
	GLint specular_uniform_[max_nr_lights];

	int get_uniform(const std::string name) const;
	int get_uniform(const std::string name, const std::string attribute, const int index) const;
public:
	MainShader();
	~MainShader();

	void init() override;
	
	
	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
	void set_light_uniforms(const std::vector<LightNode*>& light_nodes) override;
};

