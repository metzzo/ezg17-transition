#include "MainShader.h"
#include "LightNode.h"
#include "TextureResource.h"
#include "GeometryNode.h"

MainShader::MainShader() : ShaderResource("assets/shaders/main_shader.vs", "assets/shaders/main_shader.fs")
{
	this->model_uniform_ = -1;
	this->view_uniform_ = -1;
	this->projection_uniform_ = -1;
	this->material_diffuse_tex_uniform_ = -1;
	this->material_has_diffuse_tex_uniform_ = -1;
	this->material_shininess_ = -1;
	this->material_ambient_color_ = -1;
	this->material_diffuse_color_ = -1;
	this->material_specular_color_ = -1;
	this->material_material_type_ = -1;
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
	}

	for (auto i = 0; i < max_nr_shadow_maps; i++)
	{
		this->shadow_maps_uniform_[i] = -1;
		this->light_space_matrices_uniform_[i] = -1;
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
	//Check Existance of Uniforms
	assert(this->model_uniform_ >= 0);
	assert(this->material_diffuse_tex_uniform_ >= 0);
	assert(this->material_has_diffuse_tex_uniform_ >= 0);
	//Give Model to Shader
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &node->get_transformation()[0][0]);
	//Bind Texture and give it to Shader 
	auto material = node->get_mesh_resource()->get_material();
	const auto texture = material.get_texture();
	if (texture != nullptr) {
		texture->bind(0);
		glUniform1i(this->material_has_diffuse_tex_uniform_, 1);
		glUniform1i(this->material_diffuse_tex_uniform_, 0);
		glUniform1f(this->material_shininess_, material.get_shininess());
		glUniform1i(this->material_material_type_, material.get_material_type());
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUniform1i(this->material_has_diffuse_tex_uniform_, 0);
		glUniform1i(this->material_material_type_, REGULAR_MATERIAL);
	}
	glUniform3fv(this->material_ambient_color_, 1, &material.get_ambient_color()[0]);
	glUniform3fv(this->material_diffuse_color_, 1, &material.get_diffuse_color()[0]);
	glUniform3fv(this->material_specular_color_, 1, &material.get_specular_color()[0]);

}

void MainShader::set_light_uniforms(const std::vector<LightNode*>& light_nodes)
{
	auto light_index = 0;
	auto shadow_map_index = 0;
	for (auto& light : light_nodes)
	{
		assert(this->light_type_uniform_[light_index] >= 0);
		assert(this->position_uniform_[light_index] >= 0);
		assert(this->direction_uniform_[light_index] >= 0);
		assert(this->linear_uniform_[light_index] >= 0);
		assert(this->linear_uniform_[light_index] >= 0);
		assert(this->quadratic_uniform_[light_index] >= 0);
		assert(this->diffuse_uniform_[light_index] >= 0);
		assert(this->specular_uniform_[light_index] >= 0);
		assert(this->shadow_casting_uniform_[light_index] >= 0);
		assert(this->shadow_map_index_uniform_[light_index] >= 0);

		if (light->is_rendering_enabled())
		{
			// attention: if there is more than 1 diffuse texture (or specular tex)
			// then this must be increased accordingly:
			light->bind(shadow_map_index + 1);

			assert(this->shadow_maps_uniform_[shadow_map_index] >= 0);
			assert(this->light_space_matrices_uniform_[shadow_map_index] >= 0);

			auto light_space_matrix = light->get_projection_matrix() * light->get_view_matrix();

			glUniform1i(this->shadow_maps_uniform_[shadow_map_index], shadow_map_index + 1); // binds shadow map sampler
			glUniformMatrix4fv(this->light_space_matrices_uniform_[shadow_map_index], 1, GL_FALSE, &light_space_matrix[0][0]); // trafo to transform into light space
			glUniform1i(this->shadow_map_index_uniform_[light_index], shadow_map_index); // tells the exact index of the shadow map
			glUniform1i(this->shadow_casting_uniform_[light_index], 1); // boolean whether it is a shadow casting light

			shadow_map_index++;
		} else
		{
			glUniform1i(this->shadow_casting_uniform_[light_index], 0);
			
		}

		glUniform1i(this->light_type_uniform_[light_index], light->get_light_type());
		glUniform3fv(this->position_uniform_[light_index], 1, &light->get_position()[0]);
		glUniform3fv(this->direction_uniform_[light_index], 1, &light->get_direction()[0]);
		glUniform1f(this->constant_uniform_[light_index], light->get_constant());
		glUniform1f(this->linear_uniform_[light_index], light->get_linear());
		glUniform1f(this->quadratic_uniform_[light_index], light->get_quadratic());
		glUniform3fv(this->diffuse_uniform_[light_index], 1, &light->get_diffuse()[0]);
		glUniform3fv(this->specular_uniform_[light_index], 1, &light->get_specular()[0]);

		light_index++;
	}
	assert(this->num_lights_uniform_ >= 0);
	glUniform1i(this->num_lights_uniform_, light_index);
}

MainShader::~MainShader()
{
}

void MainShader::init()
{
	ShaderResource::init();

	// extract uniforms
	this->model_uniform_ = get_uniform("mvp.model");
	this->view_uniform_ = get_uniform("mvp.view");
	this->projection_uniform_ = get_uniform("mvp.projection");
	this->view_pos_uniform_ = get_uniform("view_pos");
	this->material_diffuse_tex_uniform_ = get_uniform("material.diffuse_tex");
	this->material_has_diffuse_tex_uniform_ = get_uniform("material.has_diffuse_tex");
	this->material_shininess_ = get_uniform("material.shininess");
	this->material_ambient_color_ = get_uniform("material.ambient_color");
	this->material_diffuse_color_ = get_uniform("material.diffuse_color");
	this->material_specular_color_ = get_uniform("material.specular_color");
	this->material_material_type_ = get_uniform("material.material_type");

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
	}
	for (auto i = 0; i < max_nr_shadow_maps; i++)
	{
		this->shadow_maps_uniform_[i] = get_uniform("shadow_maps", i);
		this->light_space_matrices_uniform_[i] = get_uniform("light_space_matrices", i);
	}
}
