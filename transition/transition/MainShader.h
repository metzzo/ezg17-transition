#pragma once
#include "ShaderResource.h"
#include "ILightShader.h"

class LightNode;

const unsigned int max_nr_lights = 10;
const unsigned int max_nr_directional_shadow_maps = 5;
const unsigned int max_nr_omni_directional_shadow_maps = 5;

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
	GLint shadow_casting_uniform_[max_nr_lights];
	GLint shadow_map_index_uniform_[max_nr_lights];
	GLint cutoff_uniform_[max_nr_lights];
	GLint outer_cutoff_uniform_[max_nr_lights];
	GLint far_plane_uniform_[max_nr_lights];
	GLint near_plane_uniform_[max_nr_lights];
	GLint volumetric_uniform_[max_nr_lights];
	GLint omni_directional_shadow_maps_uniform_[max_nr_omni_directional_shadow_maps];
	GLint directional_shadow_maps_uniform_[max_nr_directional_shadow_maps];
	GLint light_space_matrices_uniform_[max_nr_directional_shadow_maps];
	GLint view_pos_uniform_;
	GLint material_shininess_;
	GLint material_ambient_color_;
	GLint material_diffuse_color_;
	GLint material_specular_color_;
	GLint material_material_type_;
	int directional_shadow_map_index_;
	int omni_directional_shadow_map_index_;
	int light_index_;

	int get_texture_slot() const;
public:
	MainShader();
	~MainShader();

	void init() override;
	
	
	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;
	void set_light_uniforms(const std::vector<LightNode*>& light_nodes) override;

	void set_directional_shadow_map_uniforms(const LightNode *light, const GLint shadow_map) override;
	void set_omni_directional_shadow_map_uniforms(const LightNode *light, const GLint shadow_map, float far_plane, float near_plane) override;
};
