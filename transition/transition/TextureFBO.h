#pragma once
#include "TextureResource.h"

class TextureFBO : public TextureRenderable {

private:
	GLuint fboHandle_;
	GLuint depthBufferHandle_;
	unsigned int textureCount_;
	GLuint *textureHandles_;
	GLuint *attachments_;

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
	virtual int get_resource_id() const;

	int get_fbo_id() const;

	int get_texture_id(int index) const;

	TextureRenderable* get_texture(int index) const;

	void bind(GLuint unit, int index) const;

	void bind_for_rendering();
};