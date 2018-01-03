#pragma once
#include "TextureResource.h"

class TextureFBO : public TextureRenderable {

private:
	GLuint fbo_handle_;
	GLuint depth_buffer_handle_;
	unsigned int texture_count_;
	GLuint *texture_handles_;
	GLuint *attachments_;
	glm::ivec2 size_;

	class SingleFBOTexture : public TextureRenderable {
	private:
		GLuint texture_id_;

	public:
		SingleFBOTexture(GLuint texture_id);
		virtual int get_resource_id() const override;
	};

public:

	TextureFBO(int width, int height, unsigned int textureCount);
	virtual ~TextureFBO() override;

	/*
	Returns the ID of the first texture
	*/
	int get_resource_id() const override;

	int get_fbo_id() const;

	int get_texture_id(int index) const;

	glm::ivec2 get_size() const;

	TextureRenderable* get_texture(int index) const;

	void bind(GLuint unit, int index) const;

	void bind_for_rendering();
};