#include "TextureFBO.h"

TextureFBO::TextureFBO(int width, int height, unsigned int texture_count)
{
	this->size_ = glm::ivec2(width, height);
	this->texture_count_ = texture_count;
	this->single_fbo_textures_ = new SingleFBOTexture*[texture_count];
	for (auto i = 0; i < texture_count; i++)
	{
		this->single_fbo_textures_[i] = nullptr;
	}

	this->fbo_handle_ = 0;
	this->depth_buffer_handle_ = 0;
	this->texture_handles_ = nullptr;
}

void TextureFBO::init_color(bool depth_as_tex)
{
	texture_handles_ = new GLuint[this->texture_count_];
	glGenTextures(this->texture_count_, texture_handles_);

	if (depth_as_tex)
	{
		glBindTexture(GL_TEXTURE_2D, texture_handles_[this->texture_count_ - 1]);

		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			this->size_.x, this->size_.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	else {
		glGenRenderbuffers(1, &depth_buffer_handle_);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_handle_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->size_.x, this->size_.y);
	}

	const auto  color_attachment_count = depth_as_tex ? this->texture_count_ - 1 : this->texture_count_;

	for (unsigned int i = 0; i < color_attachment_count; i++) {
		glBindTexture(GL_TEXTURE_2D, texture_handles_[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->size_.x, this->size_.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}


	glGenFramebuffers(1, &fbo_handle_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle_);

	for (unsigned int i = 0; i < color_attachment_count; i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture_handles_[i], 0);
	}

	if (depth_as_tex)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture_handles_[this->texture_count_ - 1], 0);
	} else
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_handle_);
	}

	attachments_ = new GLuint[color_attachment_count];
	for (unsigned int i = 0; i < color_attachment_count; i++) {
		attachments_[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	if (color_attachment_count > 0) {
		glDrawBuffers(color_attachment_count, attachments_);
	} else
	{
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
	}

	for (unsigned int i = 0; i < this->texture_count_; i++) {
		this->single_fbo_textures_[i] = new SingleFBOTexture(texture_handles_[i]);
	}

	this->check_fbo();
}

void TextureFBO::init_depth()
{
	assert(this->texture_count_ == 1);

	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glGenFramebuffers(1, &this->fbo_handle_);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_handle_);

	texture_handles_ = new GLuint[this->texture_count_];
	glGenTextures(this->texture_count_, texture_handles_);

	glBindTexture(GL_TEXTURE_2D, texture_handles_[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		this->size_.x, this->size_.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture_handles_[0], 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	this->single_fbo_textures_[0] = new SingleFBOTexture(texture_handles_[0]);

	this->check_fbo();
}

void TextureFBO::check_fbo()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "TextureFBO could not be created!" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int TextureFBO::get_depth_index() const
{
	return this->texture_count_ - 1; // last texture is depth
}

TextureFBO::~TextureFBO()
{
	for (auto i = 0; i < texture_count_; i++)
	{
		delete this->single_fbo_textures_[i];
	}
	delete[] this->single_fbo_textures_;

	glDeleteTextures(texture_count_, texture_handles_);
	if (this->depth_buffer_handle_) {
		glDeleteRenderbuffers(1, &depth_buffer_handle_);
	}
	glDeleteFramebuffers(1, &fbo_handle_);
	delete[] texture_handles_;
	delete[] attachments_;
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

TextureRenderable *TextureFBO::get_texture(int index) const
{
	return single_fbo_textures_[index];
}

void TextureFBO::bind(GLuint unit, int index) const
{
	assert(texture_handles_[index] >= 0);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture_handles_[index]);
}

void TextureFBO::bind_for_rendering() const
{
	assert(fbo_handle_ >= 0);
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
