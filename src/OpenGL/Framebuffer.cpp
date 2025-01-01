#include "Framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer(CreateInfo const &createInfo)
{
    glCreateFramebuffers(1, &m_FboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

    for (int i = 0; i < createInfo.attachements.size(); i++)
    {
		currentSizeX = createInfo.attachements[i].width;
		currentSizeY = createInfo.attachements[i].height;
		screenWidth = currentSizeX;
		screenHeight = currentSizeY;
		downScaleLevel = createInfo.attachements[i].mipSizeDownscale;
		chainDepth = createInfo.attachements[i].chainDepth;

        if (GL_COLOR_ATTACHMENT0 <= createInfo.attachements[i].attachement && createInfo.attachements[i].attachement <= GL_COLOR_ATTACHMENT31)
        {
            unsigned int m_Handle = 0;
            glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
            glBindTextureUnit(0, m_Handle);

            createTextureChain(m_Handle, createInfo.attachements[i].textureFormat);
            glViewport(0, 0, createInfo.attachements[i].width, createInfo.attachements[i].height);

            m_TexIDs.push_back(m_Handle);
            m_TextureFormats.push_back(createInfo.attachements[i].textureFormat);
            m_Attachments.push_back(createInfo.attachements[i].attachement);
            glNamedFramebufferTexture(m_FboID, createInfo.attachements[i].attachement, m_TexIDs.back(), 0);
        }

        if (createInfo.attachements[i].attachement == GL_DEPTH_ATTACHMENT)
        {
			glNamedFramebufferDrawBuffer(m_FboID, GL_NONE);

            unsigned int m_Handle = 0;
            glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
            glBindTextureUnit(1, m_Handle);

            createDepthTexture(m_Handle);

            glNamedFramebufferTexture(m_FboID, GL_DEPTH_ATTACHMENT, m_Handle, 0);

            m_DepthTexIDs.push_back(m_Handle);
        }
    }

    glNamedFramebufferDrawBuffers(m_FboID, m_Attachments.size(), m_Attachments.data());

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to create framebuffer!\n";
    }

    this->unbind();
}

Framebuffer::~Framebuffer() noexcept
{
    glDeleteFramebuffers(1, &m_FboID);
    glDeleteTextures(m_TexIDs.size(), m_TexIDs.data());
    glDeleteTextures(m_DepthTexIDs.size(), m_DepthTexIDs.data());
}

void Framebuffer::createTextureChain(unsigned int &texId, const GLenum textureFormat, bool recreate, const GLenum attachment)
{
    currentSizeX = screenWidth;
    currentSizeY = screenHeight;

    if (recreate)
    {
        glDeleteTextures(1, &texId);

        glCreateTextures(GL_TEXTURE_2D, 1, &texId);
        glBindTextureUnit(0, texId);
    }

    glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(texId, chainDepth, textureFormat, currentSizeX, currentSizeY);

    if (recreate)
    {
        glNamedFramebufferTexture(m_FboID, attachment, texId, 0);
        glNamedFramebufferDrawBuffers(m_FboID, m_Attachments.size(), m_Attachments.data());
    }
}

void Framebuffer::createDepthTexture(unsigned int &depthTexID, const bool recreate) const
{
    if (recreate)
    {
        glDeleteTextures(1, &depthTexID);
        glCreateTextures(GL_TEXTURE_2D, 1, &depthTexID);
        glBindTextureUnit(1, depthTexID);
    }

    glTextureStorage2D(depthTexID, 1, GL_DEPTH_COMPONENT32F, currentSizeX, currentSizeY);

    glTextureParameteri(depthTexID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(depthTexID, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTextureParameteri(depthTexID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTexID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTexID, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    if (recreate)
    {
        glNamedFramebufferDrawBuffer(m_FboID, GL_NONE);
        glNamedFramebufferTexture(m_FboID, GL_DEPTH_ATTACHMENT, depthTexID, 0);
    }
}

void Framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
}

void Framebuffer::bindTex(const int i) const
{
    if (i > m_TexIDs.size())
    {
        std::cerr << "Index I is greater than m_TexIDS\n";
        return;
    }

    glBindTextureUnit(0, m_TexIDs[i]);
}

void Framebuffer::bindImage(const int i, const int unit) const
{
	if (i > m_TexIDs.size())
	{
		std::cerr << "Index I is greater than m_TexIDS\n";
		return;
	}

    glBindImageTexture(unit, m_TexIDs[i], 0, GL_FALSE, 0, GL_READ_WRITE, m_TextureFormats[i]);
}

void Framebuffer::bindDepthTex(const int i) const
{
	if (i > m_TexIDs.size())
	{
		std::cerr << "Index I is greater than m_DepthTexIDs\n";
		return;
	}

    glBindTextureUnit(1, m_DepthTexIDs[i]);
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::changeRes(const int width, const int height, const int i)
{
    this->bind();

    screenWidth = width;
    screenHeight = height;

    if (m_DepthTexIDs.size() > i)
    {
        this->bindDepthTex(i);
        createDepthTexture(m_DepthTexIDs[i], true);
    }
    
    if (m_TexIDs.size() > i)
    {
        this->bindTex(i);
        createTextureChain(m_TexIDs[i], m_TextureFormats[i], true, m_Attachments[i]);
    }

    this->unbind();
}