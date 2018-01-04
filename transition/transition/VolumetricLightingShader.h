#pragma once
#include "MainShader.h"
#include "TextureFBO.h"

class TextureResource;

class VolumetricLightingShader :
	public ShaderResource,
	public ILightShader
{
	GLint light_view_matrices_uniform_[max_nr_directional_shadow_maps];
	GLint light_projection_matrices_uniform_[max_nr_directional_shadow_maps];

	GLint view_inv_uniform_;
	GLint projection_inv_uniform_;
	GLint time_uniform_;

	GLint num_lights_uniform_;
	GLint light_type_uniform_[max_nr_lights];
	GLint position_uniform_[max_nr_lights];
	GLint direction_uniform_[max_nr_lights];
	GLint constant_uniform_[max_nr_lights];
	GLint linear_uniform_[max_nr_lights];
	GLint quadratic_uniform_[max_nr_lights];
	GLint diffuse_uniform_[max_nr_lights];
	GLint shadow_map_index_uniform_[max_nr_lights];
	GLint cutoff_uniform_[max_nr_lights];
	GLint outer_cutoff_uniform_[max_nr_lights];
	GLint far_plane_uniform_[max_nr_lights];
	GLint near_plane_uniform_[max_nr_lights];
	GLint omni_directional_shadow_maps_uniform_[max_nr_omni_directional_shadow_maps];
	GLint directional_shadow_maps_uniform_[max_nr_directional_shadow_maps];
	GLint phi_uniform_[max_nr_lights];
	GLint tau_uniform_[max_nr_lights];
	GLint bias_uniform_[max_nr_lights];
	GLint has_fog_uniform_[max_nr_lights];
	GLint view_pos_uniform_;
	GLint depth_texture_uniform_;

	int directional_shadow_map_index_;
	int omni_directional_shadow_map_index_;
	int light_index_;

	int get_texture_slot() const;
public:
	VolumetricLightingShader();
	~VolumetricLightingShader();

	void init() override;

	void set_camera_uniforms(const RenderingNode* node) override;
	void set_model_uniforms(const GeometryNode* node) override;

	void set_light_uniforms(const std::vector<LightNode*>& light_nodes) override;
	void set_directional_shadow_map_uniforms(const LightNode* light, const GLint shadow_map) override;
	void set_omni_directional_shadow_map_uniforms(const LightNode* light, const GLint shadow_map, float far_plane, float near_plane) override;
	void set_depth_texture(TextureRenderable *scene_tex) const;
};

