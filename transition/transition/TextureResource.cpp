#include "TextureResource.h"
#include <FreeImage.h>
#include <iostream>

TextureResource::TextureResource(const char* texture_path)
{
	int pathlen = strlen(texture_path);
	this->texture_path = new char[pathlen+1];
	for (int i = 0; i < pathlen; i++) {
		this->texture_path[i] = texture_path[i];	//herpderp, das geht sicher profesioneller
	}
	this->texture_path[pathlen] = '\0';
	this->handle = -1;
}


TextureResource::~TextureResource()
{
	if (handle != -1) {
		glDeleteTextures(1, &handle);
	}
}

int TextureResource::get_resource_id() const {
	return handle;
}

void TextureResource::init() {
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	auto formato = FreeImage_GetFileType(texture_path, 0);
	FIBITMAP* imagen = FreeImage_Load(formato, texture_path);
	unsigned int width = FreeImage_GetWidth(imagen);
	unsigned int height = FreeImage_GetHeight(imagen);
	char* data = (char*)FreeImage_GetBits(imagen);
	if (!imagen) {
		std::cerr << "Texture could not be loaded: " << texture_path << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

const char* TextureResource::get_texture_path() const {
	return this->texture_path;
}