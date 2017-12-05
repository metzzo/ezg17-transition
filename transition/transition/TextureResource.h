#pragma once
#include "IResource.h"
#include "glheaders.h"
#include <iostream>

class TextureRenderable
{
public:
	virtual ~TextureRenderable() = default;

	virtual int get_resource_id() const = 0;
	void bind(GLuint unit) const;
};

class TextureResource : public IResource, public TextureRenderable
{
public:
	explicit TextureResource(const std::string& texture_path);
	~TextureResource();

	void init() override;

	const std::string get_texture_path() const;

	int get_resource_id() const override;
private:
	std::string texture_path_;
	GLuint handle_;
};

