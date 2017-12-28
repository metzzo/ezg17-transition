#include "TextureResource.h"
#include <FreeImage.h>
#include <iostream>

void TextureRenderable::bind(GLuint unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, this->get_resource_id());
}

TextureResource::TextureResource(const std::string& texture_path)
{
	this->texture_path_ = texture_path;
	this->handle_ = -1;
}


TextureResource::~TextureResource()
{
	if (handle_ != -1) {
		glDeleteTextures(1, &handle_);
	}
}

int TextureResource::get_resource_id() const {
	return handle_;
}

void TextureResource::init() {
	glGenTextures(1, &handle_);
	glBindTexture(GL_TEXTURE_2D, handle_);

	const auto formato = FreeImage_GetFileType(texture_path_.c_str(), 0);
	FIBITMAP* imagen = FreeImage_Load(formato, texture_path_.c_str());
	const unsigned int width = FreeImage_GetWidth(imagen);
	const unsigned int height = FreeImage_GetHeight(imagen);
	char* data = reinterpret_cast<char*>(FreeImage_GetBits(imagen));
	if (!imagen) {
		std::cerr << "Texture could not be loaded: " << texture_path_.c_str() << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

const std::string TextureResource::get_texture_path() const {
	return this->texture_path_;
}

AlphaTextureResource::AlphaTextureResource(const std::string & texture_path) : TextureResource(texture_path)
{	
}

AlphaTextureResource::~AlphaTextureResource()
{
}

void AlphaTextureResource::init()
{
	glGenTextures(1, &handle_);
	glBindTexture(GL_TEXTURE_2D, handle_);

	const auto formato = FreeImage_GetFileType(texture_path_.c_str(), 0);
	FIBITMAP* imagen = FreeImage_Load(formato, texture_path_.c_str());
	const unsigned int width = FreeImage_GetWidth(imagen);
	const unsigned int height = FreeImage_GetHeight(imagen);
	char* data = reinterpret_cast<char*>(FreeImage_GetBits(imagen));
	if (!imagen) {
		std::cerr << "Texture could not be loaded: " << texture_path_.c_str() << std::endl;
	}

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}
