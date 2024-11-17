#include "Texture.h"
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <iostream>

Texture::Texture(CreateInfo const& createInfo)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(createInfo.path, &width, &height, &nrChannels, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);

	glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(m_Handle, 1, GL_RGBA8, width, height);

	if (data)
	{
		glTextureSubImage2D(m_Handle, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_Handle);
	}
	else
	{
		std::cerr << "Failed to load texture!\n";
	}

	stbi_image_free(data);
}

Texture::~Texture() noexcept
{
	glDeleteTextures(1, &m_Handle);
}

void Texture::bind(const uint8_t i) const
{
	if (0 > i || i > 31)
	{
		std::cerr << "Cant load texture unit less than 0 or more than 31!\n";
		return;
	}

	glBindTextureUnit(i, m_Handle);
}