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
	GLint material_diffuse_tex_uniform_;
	GLint material_has_diffuse_tex_uniform_;

	GLint num_lights_uniform_;
	GLint light_type_uniform_[max_nr_lights];
	GLint position_uniform_[max_nr_lights];
	GLint direction_uniform_[max_nr_lights];
	GLint constant_uniform_[max_nr_lights];
	GLint linear_uniform_[max_nr_lights];
	GLint quadratic_uniform_[max_nr_lights];
	GLint diffuse_uniform_[max_nr_lights];
	GLint specular_uniform_[max_nr_lights];
	GLint view_pos_uniform_;
	GLint material_shininess_;
	GLint material_ambient_color_;
	GLint material_diffuse_color_;
	GLint material_specular_color_;
public:
	MainShader();
	~MainShader();

	void init() override;
	
	
	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
	void set_light_uniforms(const std::vector<LightNode*>& light_nodes) override;
};

