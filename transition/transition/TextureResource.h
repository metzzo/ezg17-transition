#pragma once
#include "IResource.h"
#include "glheaders.h"
#include <iostream>

class TextureResource : public IResource
{
public:
	explicit TextureResource(const std::string& texture_path);
	~TextureResource();

	int get_resource_id() const override;
	void init() override;

	const std::string get_texture_path() const;

	void bind(GLuint unit) const;

private:
	std::string texture_path_;
	GLuint handle_;
};

