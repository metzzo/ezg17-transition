#include "ComputeShader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

ComputeShader::ComputeShader(const char * path)
{
	this->path_ = path;
	this->program_id_ = -1;
}

ComputeShader::~ComputeShader()
{
	if (this->program_id_ != -1) {
		glDeleteProgram(this->program_id_);
	}
}

void ComputeShader::use() const
{
	glUseProgram(program_id_);
}

int ComputeShader::get_resource_id() const
{
	return program_id_;
}

void ComputeShader::init()
{
	// 1. retrieve the shader source code from filePath
	std::string code;
	std::ifstream shader_file;
	// ensure ifstream objects can throw exceptions:
	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shader_file.open(this->path_);
		std::stringstream shader_stream;
		// read file's buffer contents into streams
		shader_stream << shader_file.rdbuf();
		// close file handlers
		shader_file.close();
		// convert stream into string
		code = shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		system("PAUSE");
		exit(1);
	}
	auto shader_code = code.c_str();
	// compute shader
	const auto compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &shader_code, nullptr);
	glCompileShader(compute);
	check_compile_errors(compute, "COMPUTE");
	// shader Program
	this->program_id_ = glCreateProgram();
	glAttachShader(this->program_id_, compute);
	glLinkProgram(this->program_id_);
	check_compile_errors(this->program_id_, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(compute);
}


void ComputeShader::check_compile_errors(GLuint shader, std::string type)
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
			system("PAUSE");
			exit(1);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
			system("PAUSE");
			exit(1);
		}
	}
}

GLint ComputeShader::get_uniform(const std::string name) const
{
	return glGetUniformLocation(this->get_resource_id(), name.c_str());
}

GLint ComputeShader::get_uniform(const std::string name, const int index) const
{
	return glGetUniformLocation(this->get_resource_id(), (name + "[" + std::to_string(index) + "]").c_str());
}

GLint ComputeShader::get_uniform(const std::string name, const std::string attribute, const int index) const
{
	return glGetUniformLocation(this->get_resource_id(), (name + "[" + std::to_string(index) + "]." + attribute).c_str());
}
