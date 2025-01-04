#pragma once

#include <utility>
#include <glad/glad.h>
#include <filesystem>

struct CubemapData
{
	unsigned char* data;
	int width, height, nrChannels;
};

class Texture final
{
public:
	struct CreateInfo final
	{
		std::filesystem::path path{};
		const GLenum type = GL_TEXTURE_2D;
	};

	constexpr Texture() noexcept = default;
	Texture(CreateInfo const& createInfo);
	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;
	Texture& operator=(Texture&& other) noexcept
	{
		std::swap(m_Handle, other.m_Handle);
		return *this;
	}
	inline Texture(Texture&& other) noexcept { *this = std::move(other); }
	~Texture() noexcept;

	void bind(const uint8_t i = 0) const;
	inline unsigned int getHandle() const { return m_Handle; }

private:
	unsigned int m_Handle = 69;
};