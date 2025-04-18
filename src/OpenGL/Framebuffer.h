#pragma once

#include <glad/glad.h>
#include <span>
#include <vector>

class Framebuffer final
{
public:
	struct Attachment final
	{
		const GLenum attachement=GL_COLOR_ATTACHMENT0;
	   	const int width=800, height=600;
		const GLenum textureFormat = GL_RGBA;
        const int chainDepth = 1;
	};

    struct CreateInfo final
	{
		std::span<const Attachment> attachements;
	};

	constexpr Framebuffer() noexcept = delete;
	Framebuffer(CreateInfo const &createInfo);
	Framebuffer(Framebuffer const&) = delete;
	Framebuffer& operator=(Framebuffer const&) = delete;
	Framebuffer& operator=(Framebuffer&& other) noexcept
	{
		std::swap(m_FboID, other.m_FboID);
		return *this;
	}
	inline Framebuffer(Framebuffer&& other) noexcept { *this = std::move(other); }
	~Framebuffer() noexcept;

    void bind() const;
    void unbind() const;
    void bindTex(const int i) const;
    void bindImage(const int i, const int unit) const;
    void bindDepthTex(const int i) const;

    void changeRes(const int width, const int height, const int i);

	inline unsigned int getTexHandle(const int i) const { return m_TexIDs[i]; }
	inline unsigned int getDepthTexHandle(const int i) const { return m_DepthTexIDs[i]; }

private:
    void createTextureChain(unsigned int &texId, const GLenum textureFormat, const bool recreate=false, const GLenum attachment=GL_COLOR_ATTACHMENT0);
    void createDepthTexture(unsigned int &depthTexID, const bool recreate=false) const;

    unsigned int m_FboID;
    std::vector<unsigned int> m_DepthTexIDs;
    std::vector<unsigned int> m_TexIDs;
    std::vector<GLenum> m_TextureFormats;
    std::vector<GLenum> m_Attachments;
    int chainDepth;
    int downScaleLevel;
    unsigned int screenWidth, screenHeight;
    unsigned int currentSizeX, currentSizeY;
};