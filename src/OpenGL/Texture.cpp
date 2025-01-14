#include "Texture.h"
#include <stb_image/stb_image.h>
#include <iostream>
#include <array>

Texture::Texture(CreateInfo const& createInfo)
{
	if (createInfo.path.empty()) return;

	stbi_set_flip_vertically_on_load(false);

	if (createInfo.type == GL_TEXTURE_2D)
	{
		int width{}, height{}, nrChannels{};
		unsigned char* data{};

		data = stbi_load(createInfo.path.string().c_str(), &width, &height, &nrChannels, 0);

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
	else if (createInfo.type == GL_TEXTURE_CUBE_MAP)
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_Handle);

		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::array<CubemapData, 6> cubemapData{};

		for (int i = 0; i < 6; i++)
		{
			std::string newPath = (createInfo.path.string().erase(createInfo.path.string().find(".", 0)) + std::to_string(i) + createInfo.path.extension().string());
			cubemapData[i].data = stbi_load
			(
				newPath.c_str(),
				&cubemapData[i].width,
				&cubemapData[i].height,
				&cubemapData[i].nrChannels,
				0
			);
		}

		glTextureStorage2D(m_Handle, 1, GL_RGB16F, cubemapData[0].width, cubemapData[0].height);

		for (int i = 0; i < 6; i++)
		{
			if (cubemapData[i].data)
			{
				glTextureSubImage3D(m_Handle, 0, 0, 0, i, cubemapData[i].width, cubemapData[i].height, 1, GL_RGB, GL_UNSIGNED_BYTE, cubemapData[i].data);
			}
			else
			{
				std::cerr << "Failed to load texture!\n";
			}

			stbi_image_free(cubemapData[i].data);
		}
	}
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