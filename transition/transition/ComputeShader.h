#pragma once
#include "glheaders.h"
#include "IResource.h"
#include <string>

class ComputeShader : public IResource {
private:
	const char* path_;
	GLuint program_id_;
	static void check_compile_errors(GLuint shader, std::string type);

protected:
	GLint get_uniform(const std::string name) const;
	GLint get_uniform(const std::string name, const int index) const;
	GLint get_uniform(const std::string name, const std::string attribute, const int index) const;

public:
	ComputeShader(const char *path);
	virtual ~ComputeShader();

	virtual void use() const;

	int get_resource_id() const override;
	void init() override;
};