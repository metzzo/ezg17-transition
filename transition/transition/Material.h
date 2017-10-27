#pragma once
#include <glm/glm.hpp>
#include "TextureResource.h"

class Material {
private:
	glm::vec3 ambientColor		= glm::vec3(0.0,0.0,0.0);
	glm::vec3 diffuseColor		= glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 specularColor		= glm::vec3(0.0, 0.0, 0.0);
	float shininess				= 1;
	float opacity				= 1;
	TextureResource* texture	= nullptr;

public:
	Material() {};
	glm::vec3 get_ambient_color() const {
		return ambientColor;
	}
	void set_ambient_color(const glm::vec3& color) {
		this->ambientColor = color;
	}
	glm::vec3 get_diffuse_color() const {
		return diffuseColor;
	}
	void set_diffuse_color(const glm::vec3& color) {
		this->diffuseColor = color;
	}
	glm::vec3 get_specular_color() const {
		return specularColor;
	}
	void set_specular_color(const glm::vec3& color) {
		this->specularColor = color;
	}
	float get_shininess() const {
		return shininess;
	}
	void set_shininess(float shininess) {
		this->shininess = shininess;
	}
	float get_opacity() const {
		return opacity;
	}
	void set_opacity(float opacity) {
		this->opacity = opacity;
	}
	bool has_texture() const {
		return texture != nullptr;
	}
	TextureResource* get_texture() const {
		return texture;
	}
	void set_texture(TextureResource* texture) {
		this->texture = texture;
	}
};