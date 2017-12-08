#pragma once
#include <glad/glad.h>

enum MaterialType;

class TextureRenderable
{
public:
	virtual ~TextureRenderable() = default;

	virtual int get_resource_id() const = 0;
	void bind(GLuint unit) const;

	virtual MaterialType get_material_type();
};