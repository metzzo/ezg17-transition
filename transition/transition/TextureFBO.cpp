#include "TextureFBO.h"

TextureFBO::TextureFBO(int width, int height, unsigned int textureCount)
{
	this->size_ = glm::ivec2(width, height);
	this->texture_count_ = textureCount;
	glGenFramebuffers(1, &fbo_handle_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle_);

	glGenRenderbuffers(1, &depth_buffer_handle_);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_handle_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_handle_);

	texture_handles_ = new GLuint[textureCount];
	glGenTextures(textureCount, texture_handles_);
	for (unsigned int i = 0; i < textureCount; i++) {
		glBindTexture(GL_TEXTURE_2D, texture_handles_[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture_handles_[i], 0);
	}
	attachments_ = new GLuint[textureCount];
	for (unsigned int i = 0; i < textureCount; i++) {
		attachments_[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(textureCount, attachments_);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "TextureFBO could not be created!" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureFBO::~TextureFBO()
{
	glDeleteTextures(texture_count_, texture_handles_);
	glDeleteRenderbuffers(1, &depth_buffer_handle_);
	glDeleteFramebuffers(1, &fbo_handle_);
	delete texture_handles_;
	delete attachments_;
}

int TextureFBO::get_resource_id() const
{
	return texture_handles_[0];
}

int TextureFBO::get_fbo_id() const
{
	return fbo_handle_;
}

int TextureFBO::get_texture_id(int index) const
{
	return texture_handles_[index];
}

glm::ivec2 TextureFBO::get_size() const
{
	return this->size_;
}

TextureRenderable * TextureFBO::get_texture(int index) const
{
	return new SingleFBOTexture(texture_handles_[index]);
}

void TextureFBO::bind(GLuint unit, int index) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture_handles_[index]);
}

void TextureFBO::bind_for_rendering()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle_);
}

TextureFBO::SingleFBOTexture::SingleFBOTexture(GLuint texture_id)
{
	texture_id_ = texture_id;
}

int TextureFBO::SingleFBOTexture::get_resource_id() const
{
	return texture_id_;
}
