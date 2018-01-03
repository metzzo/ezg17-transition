#include "VolumetricLightingShader.h"
#include <cassert>
#include "TextureResource.h"
#include "LightNode.h"
#include "GeometryNode.h"

static const int blue_noise_texture_slot = 0;
static const int shadow_map_texture_slot = 1;

int VolumetricLightingShader::get_texture_slot() const
{
	return this->directional_shadow_map_index_ + this->omni_directional_shadow_map_index_ + shadow_map_texture_slot;

}

VolumetricLightingShader::VolumetricLightingShader() : ShaderResource("assets/shaders/volumetric_lighting.vs", "assets/shaders/volumetric_lighting.fs")
{
	this->blue_noise_texture_uniform_ = -1;
	this->blue_noise_texture_ = new TextureResource("assets/gfx/blue_noise.png");

	this->directional_shadow_map_index_ = 0;
	this->omni_directional_shadow_map_index_ = 0;
	this->light_index_ = 0;

	this->model_uniform_ = -1;
	this->view_uniform_ = -1;
	this->projection_uniform_ = -1;
	this->view_pos_uniform_ = -1;

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
		this->specular_uniform_[i] = -1;
		this->shadow_casting_uniform_[i] = -1;
		this->shadow_map_index_uniform_[i] = -1;
		this->outer_cutoff_uniform_[i] = -1;
		this->cutoff_uniform_[i] = -1;
		this->far_plane_uniform_[i] = -1;
		this->near_plane_uniform_[i] = -1;
		this->volumetric_uniform_[i] = -1;
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

	this->seed_uniform_ = -1;
}


VolumetricLightingShader::~VolumetricLightingShader()
{
}

void VolumetricLightingShader::init()
{
	ShaderResource::init();

	// extract uniforms
	this->model_uniform_ = get_uniform("mvp.model");
	this->view_uniform_ = get_uniform("mvp.view");
	this->projection_uniform_ = get_uniform("mvp.projection");
	this->view_pos_uniform_ = get_uniform("view_pos");

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
		this->specular_uniform_[i] = get_uniform("lights", "specular", i);
		this->shadow_casting_uniform_[i] = get_uniform("lights", "shadow_casting", i);
		this->shadow_map_index_uniform_[i] = get_uniform("lights", "shadow_map_index", i);
		this->cutoff_uniform_[i] = get_uniform("lights", "cutoff", i);
		this->outer_cutoff_uniform_[i] = get_uniform("lights", "outer_cutoff", i);
		this->far_plane_uniform_[i] = get_uniform("lights", "far_plane", i);
		this->near_plane_uniform_[i] = get_uniform("lights", "near_plane", i);
		this->volumetric_uniform_[i] = get_uniform("lights", "volumetric", i);
	}

	for (auto i = 0; i < max_nr_directional_shadow_maps; i++)
	{
		this->directional_shadow_maps_uniform_[i] = get_uniform("directional_shadow_maps", i);
		this->light_projection_matrices_uniform_[i] = get_uniform("light_projection_matrices", i);
		this->light_view_matrices_uniform_[i] = get_uniform("light_view_matrices", i);
	}

	for (auto i = 0; i < max_nr_omni_directional_shadow_maps; i++)
	{
	//	this->omni_directional_shadow_maps_uniform_[i] = get_uniform("omni_directional_shadow_maps", i);
	}

	// init blue noise texture
	this->blue_noise_texture_uniform_ = get_uniform("blue_noise_texture");
	this->blue_noise_texture_->init();

	this->seed_uniform_ = get_uniform("seed");
}

void VolumetricLightingShader::set_camera_uniforms(const RenderingNode* node)
{
	assert(this->view_uniform_ >= 0);
	assert(this->projection_uniform_ >= 0);
	assert(this->view_pos_uniform_ >= 0);

	glUniformMatrix4fv(this->view_uniform_, 1, GL_FALSE, &node->get_view_matrix()[0][0]);
	glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &node->get_projection_matrix()[0][0]);
	glUniform3fv(this->view_pos_uniform_, 1, &node->get_position()[0]);
}

void VolumetricLightingShader::set_model_uniforms(const GeometryNode* node)
{
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
}

void VolumetricLightingShader::set_light_uniforms(const std::vector<LightNode*>& light_nodes)
{
	this->light_index_ = 0;
	this->directional_shadow_map_index_ = 0;
	this->omni_directional_shadow_map_index_ = 0;


	for (auto& light : light_nodes)
	{
		assert(this->light_type_uniform_[this->light_index_] >= 0);
		assert(this->position_uniform_[this->light_index_] >= 0);
		assert(this->direction_uniform_[this->light_index_] >= 0);
		assert(this->linear_uniform_[this->light_index_] >= 0);
		assert(this->linear_uniform_[this->light_index_] >= 0);
		assert(this->quadratic_uniform_[this->light_index_] >= 0);
		assert(this->diffuse_uniform_[this->light_index_] >= 0);
		assert(this->specular_uniform_[this->light_index_] >= 0);
		assert(this->shadow_casting_uniform_[this->light_index_] >= 0);
		assert(this->shadow_map_index_uniform_[this->light_index_] >= 0);
		assert(this->volumetric_uniform_[this->light_index_] >= 0);

		if (light->is_rendering_enabled())
		{
			light->set_uniforms(this);

			glUniform1i(this->shadow_casting_uniform_[this->light_index_], 1); // boolean whether it is a shadow casting light
		}
		else
		{
			glUniform1i(this->shadow_casting_uniform_[this->light_index_], 0);
		}

		glUniform1i(this->light_type_uniform_[this->light_index_], light->get_light_type());
		glUniform1i(this->volumetric_uniform_[this->light_index_], light->is_volumetric());
		glUniform3fv(this->position_uniform_[this->light_index_], 1, &light->get_position()[0]);
		glUniform3fv(this->direction_uniform_[this->light_index_], 1, &light->get_direction()[0]);
		glUniform1f(this->constant_uniform_[this->light_index_], light->get_constant());
		glUniform1f(this->linear_uniform_[this->light_index_], light->get_linear());
		glUniform1f(this->quadratic_uniform_[this->light_index_], light->get_quadratic());
		glUniform3fv(this->diffuse_uniform_[this->light_index_], 1, &light->get_diffuse()[0]);
		glUniform3fv(this->specular_uniform_[this->light_index_], 1, &light->get_specular()[0]);
		glUniform1f(this->cutoff_uniform_[this->light_index_], glm::cos(glm::radians(light->get_cutoff())));
		glUniform1f(this->outer_cutoff_uniform_[this->light_index_], glm::cos(glm::radians(light->get_outer_cutoff())));

		this->light_index_++;
	}
	assert(this->num_lights_uniform_ >= 0);
	glUniform1i(this->num_lights_uniform_, this->light_index_);

	// bind blue noise
	//assert(this->blue_noise_texture_uniform_ >= 0);
	//this->blue_noise_texture_->bind(blue_noise_texture_slot);
	//glUniform1i(this->blue_noise_texture_uniform_, blue_noise_texture_slot);
}

void VolumetricLightingShader::set_directional_shadow_map_uniforms(const LightNode* light, const GLint shadow_map)
{
	assert(this->directional_shadow_maps_uniform_[this->directional_shadow_map_index_] >= 0);

	const auto light_space_matrix = light->get_projection_matrix() * light->get_view_matrix();
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
	// TODO
	/*
	assert(this->omni_directional_shadow_maps_uniform_[this->omni_directional_shadow_map_index_] >= 0);

	const auto tex_id = get_texture_slot();
	glActiveTexture(GL_TEXTURE0 + tex_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);
	glUniform1f(this->far_plane_uniform_[this->light_index_], far_plane);
	glUniform1f(this->near_plane_uniform_[this->light_index_], near_plane);
	glUniform1i(this->omni_directional_shadow_maps_uniform_[this->omni_directional_shadow_map_index_], tex_id); // binds shadow map sampler
	glUniform1i(this->shadow_map_index_uniform_[this->light_index_], this->omni_directional_shadow_map_index_); // tells the exact index of the shadow map

	this->omni_directional_shadow_map_index_++;

	assert(this->light_view_matrices_uniform_[this->directional_shadow_map_index_] >= 0);
	assert(this->light_projection_matrices_uniform_[this->directional_shadow_map_index_] >= 0);
	glUniformMatrix4fv(this->light_view_matrices_uniform_[this->directional_shadow_map_index_], 1, GL_FALSE, &light->get_view_matrix()[0][0]); // just the view transform of light
	glUniformMatrix4fv(this->light_projection_matrices_uniform_[this->directional_shadow_map_index_], 1, GL_FALSE, &light->get_projection_matrix()[0][0]); // just the view transform of light
	*/
}

void VolumetricLightingShader::set_seed(const int seed) const {
	//assert(seed_uniform_ >= 0);
	glUniform1i(seed_uniform_, seed);
}
