#include "VolumetricLightingShader.h"
#include <cassert>
#include "TextureResource.h"
#include "LightNode.h"
#include "GeometryNode.h"

static const int depth_texture_slot = 0;
static const int shadow_map_texture_slot = 1;

int VolumetricLightingShader::get_texture_slot() const
{
	return this->directional_shadow_map_index_ + this->omni_directional_shadow_map_index_ + shadow_map_texture_slot;

}

VolumetricLightingShader::VolumetricLightingShader() : ShaderResource("assets/shaders/volumetric_lighting.vs", "assets/shaders/volumetric_lighting.fs")
{
	this->directional_shadow_map_index_ = 0;
	this->omni_directional_shadow_map_index_ = 0;
	this->light_index_ = 0;

	this->view_inv_uniform_ = -1;
	this->projection_inv_uniform_ = -1;
	this->view_pos_uniform_ = -1;
	this->time_uniform_ = -1;

	this->num_lights_uniform_ = -1;
	for (auto i = 0; i < max_nr_lights; i++)
	{
		this->light_type_uniform_[i] = -1;
		this->position_uniform_[i] = -1;
		this->direction_uniform_[i] = -1;
		this->constant_uniform_[i] = -1;
		this->linear_uniform_[i] = -1;
		this->quadratic_uniform_[i] = -1;
		this->diffuse_uniform_[i] = -1;
		this->shadow_map_index_uniform_[i] = -1;
		this->outer_cutoff_uniform_[i] = -1;
		this->cutoff_uniform_[i] = -1;
		this->far_plane_uniform_[i] = -1;
		this->near_plane_uniform_[i] = -1;
		this->phi_uniform_[i] = -1;
		this->tau_uniform_[i] = -1;
		this->bias_uniform_[i] = -1;
		this->has_fog_uniform_[i] = -1;
	}

	for (auto i = 0; i < max_nr_directional_shadow_maps; i++)
	{
		this->directional_shadow_maps_uniform_[i] = -1;
		this->light_view_matrices_uniform_[i] = -1;
		this->light_projection_matrices_uniform_[i] = -1;
	}

	for (auto i = 0; i < max_nr_omni_directional_shadow_maps; i++)
	{
		this->omni_directional_shadow_maps_uniform_[i] = -1;
	}

	this->depth_texture_uniform_ = -1;
}


VolumetricLightingShader::~VolumetricLightingShader()
{
}

void VolumetricLightingShader::init()
{
	ShaderResource::init();

	// extract uniforms
	this->view_inv_uniform_ = get_uniform("vp.view_inv");
	this->projection_inv_uniform_ = get_uniform("vp.projection_inv");
	this->view_pos_uniform_ = get_uniform("view_pos");
	this->time_uniform_ = get_uniform("time");

	this->num_lights_uniform_ = get_uniform("num_lights");

	for (auto i = 0; i < max_nr_lights; i++)
	{
		this->light_type_uniform_[i] = get_uniform("lights", "light_type", i);
		this->position_uniform_[i] = get_uniform("lights", "position", i);
		this->direction_uniform_[i] = get_uniform("lights", "direction", i);
		this->constant_uniform_[i] = get_uniform("lights", "constant", i);
		this->linear_uniform_[i] = get_uniform("lights", "linear", i);
		this->quadratic_uniform_[i] = get_uniform("lights", "quadratic", i);
		this->diffuse_uniform_[i] = get_uniform("lights", "diffuse", i);
		this->shadow_map_index_uniform_[i] = get_uniform("lights", "shadow_map_index", i);
		this->cutoff_uniform_[i] = get_uniform("lights", "cutoff", i);
		this->outer_cutoff_uniform_[i] = get_uniform("lights", "outer_cutoff", i);
		this->far_plane_uniform_[i] = get_uniform("lights", "far_plane", i);
		this->near_plane_uniform_[i] = get_uniform("lights", "near_plane", i);
		this->phi_uniform_[i] = get_uniform("lights", "phi", i);
		this->tau_uniform_[i] = get_uniform("lights", "tau", i);
		this->bias_uniform_[i] = get_uniform("lights", "bias", i);
		this->has_fog_uniform_[i] = get_uniform("lights", "has_fog", i);
	}

	for (auto i = 0; i < max_nr_directional_shadow_maps; i++)
	{
		this->directional_shadow_maps_uniform_[i] = get_uniform("directional_shadow_maps", i);
		this->light_projection_matrices_uniform_[i] = get_uniform("light_projection_matrices", i);
		this->light_view_matrices_uniform_[i] = get_uniform("light_view_matrices", i);
	}

	for (auto i = 0; i < max_nr_omni_directional_shadow_maps; i++)
	{
		this->omni_directional_shadow_maps_uniform_[i] = get_uniform("omni_directional_shadow_maps", i);
	}

	this->depth_texture_uniform_ = get_uniform("depth_tex");
}

void VolumetricLightingShader::set_camera_uniforms(const RenderingNode* node)
{
	assert(this->view_inv_uniform_ >= 0);
	assert(this->projection_inv_uniform_ >= 0);
	assert(this->view_pos_uniform_ >= 0);

	glUniform1f(this->time_uniform_, glfwGetTime());
	glUniformMatrix4fv(this->view_inv_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
	glUniformMatrix4fv(this->projection_inv_uniform_, 1, GL_FALSE, &node->get_projection_inverse_matrix()[0][0]);
	glUniform3fv(this->view_pos_uniform_, 1, &node->get_position()[0]);
}

void VolumetricLightingShader::set_model_uniforms(const GeometryNode* node)
{
}

void VolumetricLightingShader::set_light_uniforms(const std::vector<LightNode*>& light_nodes)
{
	this->light_index_ = 0;
	this->directional_shadow_map_index_ = 0;
	this->omni_directional_shadow_map_index_ = 0;

	for (auto& light : light_nodes)
	{
		// only volumetric lights should be passed to the shader and whose lights have a shadow map
		if (!light->is_volumetric() || !light->is_rendering_enabled())
		{
			continue;
		}

		assert(this->light_type_uniform_[this->light_index_] >= 0);
		assert(this->position_uniform_[this->light_index_] >= 0);
		assert(this->direction_uniform_[this->light_index_] >= 0);
		assert(this->constant_uniform_[this->light_index_] >= 0);
		assert(this->linear_uniform_[this->light_index_] >= 0);
		assert(this->quadratic_uniform_[this->light_index_] >= 0);
		assert(this->diffuse_uniform_[this->light_index_] >= 0);
		assert(this->shadow_map_index_uniform_[this->light_index_] >= 0);
		assert(this->phi_uniform_[this->light_index_] >= 0);
		assert(this->tau_uniform_[this->light_index_] >= 0);
		assert(this->bias_uniform_[this->light_index_] >= 0);
		assert(this->has_fog_uniform_[this->light_index_] >= 0);

		light->set_uniforms(this);

		glUniform1i(this->light_type_uniform_[this->light_index_], light->get_light_type());
		glUniform3fv(this->position_uniform_[this->light_index_], 1, &light->get_position()[0]);
		glUniform3fv(this->direction_uniform_[this->light_index_], 1, &light->get_direction()[0]);
		glUniform1f(this->constant_uniform_[this->light_index_], light->get_constant());
		glUniform1f(this->linear_uniform_[this->light_index_], light->get_linear());
		glUniform1f(this->quadratic_uniform_[this->light_index_], light->get_quadratic());
		glUniform3fv(this->diffuse_uniform_[this->light_index_], 1, &light->get_diffuse()[0]);
		glUniform1f(this->cutoff_uniform_[this->light_index_], glm::cos(glm::radians(light->get_cutoff())));
		glUniform1f(this->outer_cutoff_uniform_[this->light_index_], glm::cos(glm::radians(light->get_outer_cutoff())));
		glUniform1f(this->phi_uniform_[this->light_index_], light->get_phi());
		glUniform1f(this->tau_uniform_[this->light_index_], light->get_tau());
		glUniform1f(this->bias_uniform_[this->light_index_], light->get_min_bias());
		glUniform1i(this->has_fog_uniform_[this->light_index_], light->has_fog());

		this->light_index_++;
	}
	assert(this->num_lights_uniform_ >= 0);
	glUniform1i(this->num_lights_uniform_, this->light_index_);
}

void VolumetricLightingShader::set_directional_shadow_map_uniforms(const LightNode* light, const GLint shadow_map)
{
	assert(this->directional_shadow_maps_uniform_[this->directional_shadow_map_index_] >= 0);

	const auto tex_id = get_texture_slot();
	glActiveTexture(GL_TEXTURE0 + tex_id);
	glBindTexture(GL_TEXTURE_2D, shadow_map);

	glUniform1i(this->directional_shadow_maps_uniform_[this->directional_shadow_map_index_], tex_id); // binds shadow map sampler
	glUniform1i(this->shadow_map_index_uniform_[this->light_index_], this->directional_shadow_map_index_); // tells the exact index of the shadow map
	
	assert(this->light_view_matrices_uniform_[this->directional_shadow_map_index_] >= 0);
	assert(this->light_projection_matrices_uniform_[this->directional_shadow_map_index_] >= 0);
	glUniformMatrix4fv(this->light_projection_matrices_uniform_[this->directional_shadow_map_index_], 1, GL_FALSE, &light->get_projection_matrix()[0][0]); // just the view transform of light
	glUniformMatrix4fv(this->light_view_matrices_uniform_[this->directional_shadow_map_index_], 1, GL_FALSE, &light->get_view_matrix()[0][0]); // just the view transform of light

	this->directional_shadow_map_index_++;
}

void VolumetricLightingShader::set_omni_directional_shadow_map_uniforms(const LightNode* light, const GLint shadow_map,
	float far_plane, float near_plane)
{
	assert(this->omni_directional_shadow_maps_uniform_[this->omni_directional_shadow_map_index_] >= 0);
	assert(this->far_plane_uniform_[this->light_index_]);

	const auto tex_id = get_texture_slot();
	glActiveTexture(GL_TEXTURE0 + tex_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);
	glUniform1f(this->far_plane_uniform_[this->light_index_], far_plane);
	glUniform1f(this->near_plane_uniform_[this->light_index_], near_plane);
	glUniform1i(this->omni_directional_shadow_maps_uniform_[this->omni_directional_shadow_map_index_], tex_id); // binds shadow map sampler
	glUniform1i(this->shadow_map_index_uniform_[this->light_index_], this->omni_directional_shadow_map_index_); // tells the exact index of the shadow map
	
	this->omni_directional_shadow_map_index_++;
}

void VolumetricLightingShader::set_depth_texture(TextureRenderable* scene_tex) const
{
	glUniform1i(this->depth_texture_uniform_, depth_texture_slot);
	scene_tex->bind(depth_texture_slot);
}
