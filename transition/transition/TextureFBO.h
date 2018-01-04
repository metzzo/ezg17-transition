#pragma once
#include "TextureResource.h"

class TextureFBO : public TextureRenderable {
	class SingleFBOTexture : public TextureRenderable {
		GLuint texture_id_;

	public:
		explicit SingleFBOTexture(GLuint texture_id);
		int get_resource_id() const override;
	};

	GLuint fbo_handle_;
	GLuint depth_buffer_handle_;
	unsigned int texture_count_;
	GLuint *texture_handles_;
	SingleFBOTexture **single_fbo_textures_;
	GLuint *attachments_;
	glm::ivec2 size_;
public:

	TextureFBO(int width, int height, unsigned int texture_count);
	~TextureFBO() override;

	void init_color(bool depth_as_tex = false);
	void init_depth();
	static void check_fbo();

	int get_depth_index() const;

	/*
	Returns the ID of the first texture
	*/
	int get_resource_id() const override;

	int get_fbo_id() const;

	int get_texture_id(int index) const;

	glm::ivec2 get_size() const;

	TextureRenderable* get_texture(int index) const;

	void bind(GLuint unit, int index) const;

	void bind_for_rendering() const;
};