#pragma once
#include "IResource.h"
#include "glheaders.h"
#include <iostream>
#include "Material.h"
#include "TextureRenderable.h"

class TextureResource : public IResource, public TextureRenderable
{
public:
	explicit TextureResource(const std::string& texture_path);
	virtual ~TextureResource();

	virtual void init() override;

	const std::string get_texture_path() const;

	int get_resource_id() const override;
protected:
	std::string texture_path_;
	GLuint handle_;
};

class AlphaTextureResource : public TextureResource {
public:
	explicit AlphaTextureResource(const std::string& texture_path);
	~AlphaTextureResource();

	void init() override;
};