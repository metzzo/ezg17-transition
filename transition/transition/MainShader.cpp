#include "MainShader.h"
#include "LightNode.h"
#include "TextureResource.h"
#include "GeometryNode.h"

static const int diffuse_texture_slot = 0;
static const int alpha_texture_slot = 1;
static const int shadow_map_texture_slot = 2;

int MainShader::get_texture_slot() const
{
	return this->directional_shadow_map_index_ + this->omni_directional_shadow_map_index_ + shadow_map_texture_slot;
}

MainShader::MainShader(const char* vertex_path, const char* fragment_path, const char* geometry_path) : ShaderResource(vertex_path, fragment_path, geometry_path)
{
	this->directional_shadow_map_index_ = 0;
	this->omni_directional_shadow_map_index_ = 0;
	this->light_index_ = 0;

	this->model_uniform_ = -1;
	this->model_normal_uniform_ = -1;
	this->view_uniform_ = -1;
	this->projection_uniform_ = -1;
	this->material_diffuse_tex_uniform_ = -1;
	this->material_has_diffuse_tex_uniform_ = -1;
	this->material_alpha_tex_uniform_ = -1;
	this->material_has_alpha_tex_uniform = -1;
	this->material_shininess_ = -1;
	this->material_ambient_color_ = -1;
	this->material_diffuse_color_ = -1;
	this->material_specular_color_ = -1;
	this->material_material_type_ = -1;
	this->material_opacity_ = -1;
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
		this->shadow_min_bias_[i] = -1;
		this->shadow_max_bias_[i] = -1;
	}

	for (auto i = 0; i < max_nr_directional_shadow_maps; i++)
	{
		this->directional_shadow_maps_uniform_[i] = -1;
		this->light_space_matrices_uniform_[i] = -1;
	}

	for (auto i = 0; i < max_nr_omni_directional_shadow_maps; i++)
	{
		this->omni_directional_shadow_maps_uniform_[i] = -1;
	}
}


void MainShader::set_camera_uniforms(const RenderingNode* node) {
	assert(this->view_uniform_ >= 0);
	assert(this->projection_uniform_ >= 0);
	assert(this->view_pos_uniform_ >= 0);

	glUniformMatrix4fv(this->view_uniform_, 1, GL_FALSE, &node->get_view_matrix()[0][0]);
	glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &node->get_projection_matrix()[0][0]);
	glUniform3fv(this->view_pos_uniform_, 1, &node->get_position()[0]);
}

void MainShader::set_model_uniforms(const GeometryNode* node) {
	// Check Existance of Uniforms
	assert(this->model_uniform_ >= 0);
	assert(this->material_diffuse_tex_uniform_ >= 0);
	assert(this->material_has_diffuse_tex_uniform_ >= 0);
	// Give Model to Shader
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);

	// and bind the model normal
	auto model_normal = glm::mat3(glm::transpose(node->get_inverse_transformation()));
	glUniformMatrix3fv(this->model_normal_uniform_, 1, GL_FALSE, &model_normal[0][0]);


	// Bind Texture and give it to Shader 
	auto material = node->get_mesh_resource()->get_material();
	const auto texture = material.get_texture();
	if (texture != nullptr) {
		texture->bind(diffuse_texture_slot);
		glUniform1i(this->material_has_diffuse_tex_uniform_, 1);
		glUniform1i(this->material_diffuse_tex_uniform_, diffuse_texture_slot);
		glUniform1f(this->material_shininess_, material.get_shininess());
		glUniform1i(this->material_material_type_, material.get_material_type());
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUniform1i(this->material_has_diffuse_tex_uniform_, 0);
		glUniform1i(this->material_material_type_, REGULAR_MATERIAL);
	}
	if (material.has_alpha_texture()) {
		auto alpha = material.get_alpha_texture();
		alpha->bind(alpha_texture_slot);
		glUniform1i(this->material_has_alpha_tex_uniform, 1);
		glUniform1i(this->material_alpha_tex_uniform_, alpha_texture_slot);
	}
	else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUniform1i(this->material_has_alpha_tex_uniform, 0);
	}
	glUniform3fv(this->material_ambient_color_, 1, &material.get_ambient_color()[0]);
	glUniform3fv(this->material_diffuse_color_, 1, &material.get_diffuse_color()[0]);
	glUniform3fv(this->material_specular_color_, 1, &material.get_specular_color()[0]);
	glUniform1f(this->material_opacity_, material.get_opacity());

}

void MainShader::set_light_uniforms(const std::vector<LightNode*>& light_nodes)
{
	this->light_index_ = 0;
	this->directional_shadow_map_index_ = 0;
	this->omni_directional_shadow_map_index_ = 0;


	for (auto& light : light_nodes)
	{
		if (light->is_enabled()) {
			assert(this->light_type_uniform_[this->light_index_] >= 0);
			assert(this->position_uniform_[this->light_index_] >= 0);
			assert(this->direction_uniform_[this->light_index_] >= 0);
			assert(this->constant_uniform_[this->light_index_] >= 0);
			assert(this->linear_uniform_[this->light_index_] >= 0);
			assert(this->quadratic_uniform_[this->light_index_] >= 0);
			assert(this->diffuse_uniform_[this->light_index_] >= 0);
			assert(this->specular_uniform_[this->light_index_] >= 0);
			assert(this->shadow_casting_uniform_[this->light_index_] >= 0);
			assert(this->shadow_map_index_uniform_[this->light_index_] >= 0);
			assert(this->shadow_min_bias_[this->light_index_] >= 0);
			assert(this->shadow_max_bias_[this->light_index_] >= 0);

			if (light->is_rendering_enabled())
			{
				light->set_uniforms(this);

				glUniform1f(this->shadow_min_bias_[this->light_index_], light->get_min_bias());
				glUniform1f(this->shadow_max_bias_[this->light_index_], light->get_max_bias());
				glUniform1i(this->shadow_casting_uniform_[this->light_index_], 1); // boolean whether it is a shadow casting light
			}
			else
			{
				glUniform1i(this->shadow_casting_uniform_[this->light_index_], 0);
			}

			glUniform1i(this->light_type_uniform_[this->light_index_], light->get_light_type());
			glUniform3fv(this->position_uniform_[this->light_index_], 1, &light->get_position()[0]);
			glUniform3fv(this->direction_uniform_[this->light_index_], 1, &light->get_direction()[0]);
			glUniform1f(this->constant_uniform_[this->light_index_], light->get_constant());
			glUniform1f(this->linear_uniform_[this->light_index_], light->get_linear());
			glUniform1f(this->quadratic_uniform_[this->light_index_], light->get_quadratic());
			glUniform3fv(this->diffuse_uniform_[this->light_index_], 1, &light->get_diffuse()[0]);
			glUniform3fv(this->specular_uniform_[this->light_index_], 1, &light->get_specular()[0]);

			if (light->get_light_type() == SPOT_LIGHT) {
				glUniform1f(this->cutoff_uniform_[this->light_index_], glm::cos(glm::radians(light->get_cutoff())));
				glUniform1f(this->outer_cutoff_uniform_[this->light_index_], glm::cos(glm::radians(light->get_outer_cutoff())));
			}

			this->light_index_++;
		}
	}
	assert(this->num_lights_uniform_ >= 0);
	glUniform1i(this->num_lights_uniform_, this->light_index_);
}

void MainShader::set_directional_shadow_map_uniforms(const LightNode *light, const GLint shadow_map)
{
	assert(this->directional_shadow_maps_uniform_[this->directional_shadow_map_index_] >= 0);
	assert(this->light_space_matrices_uniform_[this->directional_shadow_map_index_] >= 0);

	const auto light_space_matrix = light->get_projection_matrix() * light->get_view_matrix();
	const auto tex_id = get_texture_slot();
	glActiveTexture(GL_TEXTURE0 + tex_id);
	glBindTexture(GL_TEXTURE_2D, shadow_map);

	glUniform1i(this->directional_shadow_maps_uniform_[this->directional_shadow_map_index_], tex_id); // binds shadow map sampler
	glUniformMatrix4fv(this->light_space_matrices_uniform_[this->directional_shadow_map_index_], 1, GL_FALSE, &light_space_matrix[0][0]); // trafo to transform into light space
	glUniform1i(this->shadow_map_index_uniform_[this->light_index_], this->directional_shadow_map_index_); // tells the exact index of the shadow map

	this->directional_shadow_map_index_++;
}

void MainShader::set_omni_directional_shadow_map_uniforms(const LightNode* light, const GLint shadow_map, const float far_plane, const float near_plane)
{
	assert(this->omni_directional_shadow_maps_uniform_[this->omni_directional_shadow_map_index_] >= 0);

	const auto tex_id = get_texture_slot();
	glActiveTexture(GL_TEXTURE0 + tex_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);
	glUniform1f(this->far_plane_uniform_[this->light_index_], far_plane);
	glUniform1f(this->near_plane_uniform_[this->light_index_], near_plane);
	glUniform1i(this->omni_directional_shadow_maps_uniform_[this->omni_directional_shadow_map_index_], tex_id); // binds shadow map sampler
	glUniform1i(this->shadow_map_index_uniform_[this->light_index_], this->omni_directional_shadow_map_index_); // tells the exact index of the shadow map

	this->omni_directional_shadow_map_index_++;
}

MainShader::~MainShader()
{
}

void MainShader::init()
{
	ShaderResource::init();

	// extract uniforms
	this->model_uniform_ = get_uniform("mvp.model");
	this->model_normal_uniform_ = get_uniform("mvp.model_normal");
	this->view_uniform_ = get_uniform("mvp.view");
	this->projection_uniform_ = get_uniform("mvp.projection");
	this->view_pos_uniform_ = get_uniform("view_pos");
	this->material_diffuse_tex_uniform_ = get_uniform("material.diffuse_tex");
	this->material_has_diffuse_tex_uniform_ = get_uniform("material.has_diffuse_tex");
	this->material_alpha_tex_uniform_ = get_uniform("material.alpha_tex");
	this->material_has_alpha_tex_uniform = get_uniform("material.has_alpha_tex");
	this->material_shininess_ = get_uniform("material.shininess");
	this->material_ambient_color_ = get_uniform("material.ambient_color");
	this->material_diffuse_color_ = get_uniform("material.diffuse_color");
	this->material_specular_color_ = get_uniform("material.specular_color");
	this->material_material_type_ = get_uniform("material.material_type");
	this->material_opacity_ = get_uniform("material.opacity");

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
		this->shadow_min_bias_[i] = get_uniform("lights", "min_bias", i);
		this->shadow_max_bias_[i] = get_uniform("lights", "max_bias", i);
	}

	for (auto i = 0; i < max_nr_directional_shadow_maps; i++)
	{
		this->directional_shadow_maps_uniform_[i] = get_uniform("directional_shadow_maps", i);
		this->light_space_matrices_uniform_[i] = get_uniform("light_space_matrices", i);
	}

	for (auto i = 0; i < max_nr_omni_directional_shadow_maps; i++)
	{
		this->omni_directional_shadow_maps_uniform_[i] = get_uniform("omni_directional_shadow_maps", i);
	}
}
