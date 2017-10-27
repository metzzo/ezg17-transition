#include "ShaderResource.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

ShaderResource::ShaderResource(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
	this->vertex_path_ = vertex_path;
	this->fragment_path_ = fragment_path;
	this->geometry_path_ = geometry_path;
	this->program_id_ = -1;
}

ShaderResource::~ShaderResource()
{
	if (this->program_id_ != -1)
	{
		glDeleteProgram(this->program_id_);
	}
}

void ShaderResource::use() const
{
	const auto program_id = this->get_resource_id();
	glUseProgram(program_id);
}

int ShaderResource::get_resource_id() const
{
	return this->program_id_;
}

void ShaderResource::init()
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	std::ifstream g_shader_file;
	// ensure ifstream objects can throw exceptions:
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		v_shader_file.open(this->vertex_path_);
		f_shader_file.open(this->fragment_path_);
		std::stringstream v_shader_stream, f_shader_stream;
		// read file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		// close file handlers
		v_shader_file.close();
		f_shader_file.close();
		// convert stream into string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
		// if geometry shader path is present, also load a geometry shader
		if (this->geometry_path_ != nullptr)
		{
			g_shader_file.open(this->geometry_path_);
			std::stringstream gShaderStream;
			gShaderStream << g_shader_file.rdbuf();
			g_shader_file.close();
			geometry_code = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	auto v_shader_code = vertex_code.c_str();
	auto f_shader_code = fragment_code.c_str();
	// vertex shader
	const auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, nullptr);
	glCompileShader(vertex);
	check_compile_errors(vertex, "VERTEX");
	// fragment Shader
	const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, nullptr);
	glCompileShader(fragment);
	check_compile_errors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry = 0;
	if (this->geometry_path_ != nullptr)
	{
		auto g_shader_code = geometry_code.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &g_shader_code, nullptr);
		glCompileShader(geometry);
		check_compile_errors(geometry, "GEOMETRY");
	}
	// shader Program
	this->program_id_ = glCreateProgram();
	glAttachShader(this->program_id_, vertex);
	glAttachShader(this->program_id_, fragment);
	if (this->geometry_path_ != nullptr) 
	{
		glAttachShader(this->program_id_, geometry);
	}
	glLinkProgram(this->program_id_);
	check_compile_errors(this->program_id_, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (this->geometry_path_ != nullptr)
	{
		glDeleteShader(geometry);
	}

}


void ShaderResource::check_compile_errors(const GLuint shader, const std::string type)
{
	GLint success;
	GLchar info_log[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

GLint ShaderResource::get_uniform(const std::string name) const
{
	return glGetUniformLocation(this->ShaderResource::get_resource_id(), name.c_str());
}

GLint ShaderResource::get_uniform(const std::string name, const std::string attribute, const int index) const
{
	return glGetUniformLocation(this->ShaderResource::get_resource_id(), (name + "[" + std::to_string(index) + "]." + attribute).c_str());
}
