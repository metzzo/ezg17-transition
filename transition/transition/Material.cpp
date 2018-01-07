#include "Material.h"
#include "TextureResource.h"

glm::vec3 Material::get_ambient_color() const
{
	return ambient_color_;
}

void Material::set_ambient_color(const glm::vec3& color)
{
	this->ambient_color_ = color;
}

glm::vec3 Material::get_diffuse_color() const
{
	return diffuse_color_;
}

void Material::set_diffuse_color(const glm::vec3& color)
{
	this->diffuse_color_ = color;
}

glm::vec3 Material::get_specular_color() const
{
	return specular_color_;
}

void Material::set_specular_color(const glm::vec3& color)
{
	this->specular_color_ = color;
}

float Material::get_shininess() const
{
	return shininess_;
}

void Material::set_shininess(const float shininess)
{
	this->shininess_ = shininess;
}

float Material::get_opacity() const
{
	return opacity_;
}

void Material::set_opacity(const float opacity)
{
	this->opacity_ = opacity;
}

bool Material::has_texture() const
{
	return texture_ != nullptr;
}

TextureRenderable* Material::get_texture() const
{
	return texture_;
}

void Material::set_texture(TextureRenderable* texture)
{
	this->texture_ = texture;
}

bool Material::has_alpha_texture() const
{
	return (alpha_texture_ != nullptr);
}

TextureRenderable * Material::get_alpha_texture() const
{
	return alpha_texture_;
}

void Material::set_alpha_texture(TextureRenderable * texture)
{
	alpha_texture_ = texture;
}

MaterialType Material::get_material_type() const
{
	if (this->texture_ != nullptr) {
		return this->texture_->get_material_type();
	} else
	{
		return REGULAR_MATERIAL;
	}
}

float Material::get_alpha_cutoff() const
{
	return alpha_cutoff_;
}

void Material::set_alpha_cutoff(float cutoff)
{
	alpha_cutoff_ = cutoff;
}

