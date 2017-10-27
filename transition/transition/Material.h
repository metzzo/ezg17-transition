#pragma once
#include <glm/glm.hpp>
#include "TextureResource.h"

class Material {
private:
	glm::vec3 ambient_color_		= glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 diffuse_color_		= glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 specular_color_		= glm::vec3(0.0, 0.0, 0.0);
	float shininess_				= 1;
	float opacity_					= 1;
	TextureResource* texture_		= nullptr;

public:
	Material() {};
	glm::vec3 get_ambient_color() const {
		return ambient_color_;
	}
	void set_ambient_color(const glm::vec3& color) {
		this->ambient_color_ = color;
	}
	glm::vec3 get_diffuse_color() const {
		return diffuse_color_;
	}
	void set_diffuse_color(const glm::vec3& color) {
		this->diffuse_color_ = color;
	}
	glm::vec3 get_specular_color() const {
		return specular_color_;
	}
	void set_specular_color(const glm::vec3& color) {
		this->specular_color_ = color;
	}
	float get_shininess() const {
		return shininess_;
	}
	void set_shininess(const float shininess) {
		this->shininess_ = shininess;
	}
	float get_opacity() const {
		return opacity_;
	}
	void set_opacity(const float opacity) {
		this->opacity_ = opacity;
	}
	bool has_texture() const {
		return texture_ != nullptr;
	}
	TextureResource* get_texture() const {
		return texture_;
	}
	void set_texture(TextureResource* texture) {
		this->texture_ = texture;
	}
};