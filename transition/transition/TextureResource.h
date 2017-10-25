#pragma once
#include "IResource.h"
#include "glheaders.h"

class TextureResource : public IResource
{
public:
	TextureResource(const char* texture_path);
	~TextureResource();

	int get_resource_id() const override;
	void init() override;

	const char* get_texture_path() const;

	void bind(GLuint unit);

private:
	char* texture_path_;
	GLuint handle_;
};

