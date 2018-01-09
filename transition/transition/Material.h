#pragma once
#include <glm/glm.hpp>

class TextureRenderable;

enum MaterialType
{
	REGULAR_MATERIAL = 0,
	DEBUG_DEPTH_MATERIAL = 1
};

class Material {
private:
	glm::vec3 ambient_color_			= glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 diffuse_color_			= glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 specular_color_			= glm::vec3(0.0, 0.0, 0.0);
	float shininess_					= 1;
	float opacity_						= 1;
	TextureRenderable* texture_			= nullptr;
	TextureRenderable* alpha_texture_	= nullptr;
	float alpha_cutoff_					= 0.8;
public:
	Material()
	{
		this->texture_ = nullptr;
		this->alpha_texture_ = nullptr;
	};

	glm::vec3 get_ambient_color() const;
	void set_ambient_color(const glm::vec3& color);

	glm::vec3 get_diffuse_color() const;
	void set_diffuse_color(const glm::vec3& color);

	glm::vec3 get_specular_color() const;
	void set_specular_color(const glm::vec3& color);

	float get_shininess() const;
	void set_shininess(const float shininess);

	float get_opacity() const;
	void set_opacity(const float opacity);

	bool has_texture() const;
	TextureRenderable* get_texture() const;
	void set_texture(TextureRenderable* texture);

	bool has_alpha_texture() const;
	TextureRenderable* get_alpha_texture() const;
	void set_alpha_texture(TextureRenderable* texture);

	MaterialType get_material_type() const;

	float get_alpha_cutoff() const;
	void set_alpha_cutoff(float cutoff);
};