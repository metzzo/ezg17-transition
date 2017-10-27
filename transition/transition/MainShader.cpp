#include "MainShader.h"


MainShader::MainShader() : ShaderResource("assets/shaders/main_shader.vs", "assets/shaders/main_shader.fs")
{
	this->model_uniform_ = -1;
	this->view_uniform_ = -1;
	this->projection_uniform_ = -1;
	this->diffuse_texture_uniform_ = -1;
}


void MainShader::set_camera_uniforms(const RenderingNode* node) {
	assert(this->view_uniform_ >= 0);
	assert(this->projection_uniform_ >= 0);
	glUniformMatrix4fv(this->view_uniform_, 1, GL_FALSE, &(node->get_view_matrix())[0][0]);
	glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &(node->get_projection_matrix())[0][0]);
}

void MainShader::set_model_uniforms(const GeometryNode* node) {
	//Check Existance of Uniforms
	assert(this->model_uniform_ >= 0);
	assert(this->diffuse_texture_uniform_ >= 0);
	//Give Model to Shader
	glUniformMatrix4fv(this->model_uniform_, 1, GL_FALSE, &(node->get_transformation())[0][0]);
	//Bind Texture and give it to Shader 
	auto material = node->get_mesh_resource()->get_material();
	auto texture = material.get_texture();
	texture->bind(0);
	glUniform1i(this->diffuse_texture_uniform_, 0);
}

void MainShader::set_light_uniforms(const std::vector<LightNode*>& light_nodes)
{
	// TODO
}


MainShader::~MainShader()
{
}

void MainShader::init()
{
	ShaderResource::init();

	// extract uniforms
	this->model_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "model");
	this->view_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "view");
	this->projection_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "projection");
	this->diffuse_texture_uniform_ = glGetUniformLocation(this->ShaderResource::get_resource_id(), "diffusetexture");
}
