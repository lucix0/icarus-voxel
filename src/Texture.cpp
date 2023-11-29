#include "Texture.h"
#include <stb_image/stb_image.h>
#include <glad/glad.h>

Texture::Texture() : m_handle(-1) {  }

void Texture::load_from_file(const std::string& path) {
	glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);

	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nr_channels;	
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
	if (data) {
		glTextureStorage2D(m_handle, 1, GL_RGB8, width, height);
		glTextureSubImage2D(m_handle, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_handle);
		stbi_image_free(data);
	}
}

unsigned int Texture::get_handle() {
	return m_handle;
}
