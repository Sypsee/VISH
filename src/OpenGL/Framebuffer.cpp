#include "Framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer(CreateInfo const &createInfo)
{
    glCreateFramebuffers(1, &m_FboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

    for (int i = 0; i < createInfo.attachements.size(); i++)
    {
        if (GL_COLOR_ATTACHMENT0 <= createInfo.attachements[i].attachement && createInfo.attachements[i].attachement <= GL_COLOR_ATTACHMENT31)
        {
            currentSizeX = createInfo.attachements[i].width;
            currentSizeY = createInfo.attachements[i].height;
            screenWidth = currentSizeX;
            screenHeight = currentSizeY;
            downScaleLevel = createInfo.attachements[i].mipSizeDownscale;
            chainDepth = createInfo.attachements[i].chainDepth;

            unsigned int m_Handle = 0;
            glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
            glBindTextureUnit(0, m_Handle);

            createTextureChain(m_Handle);
            glViewport(0, 0, createInfo.attachements[i].width, createInfo.attachements[i].height);

            m_TexIDs.push_back(m_Handle);
            glNamedFramebufferTexture(m_FboID, createInfo.attachements[i].attachement, m_TexIDs.back(), 0);
            
            unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
            glDrawBuffers(1, attachments);
        }

        if (createInfo.attachements[i].attachement == GL_DEPTH_ATTACHMENT)
        {
            unsigned int m_Handle = 0;
            glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
            glBindTextureUnit(1, m_Handle);

            createDepthTexture(m_Handle);

            glNamedFramebufferTexture(m_FboID, GL_DEPTH_ATTACHMENT, m_Handle, 0);

            m_DepthTexIDs.push_back(m_Handle);
        }
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to create framebuffer!\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() noexcept
{
    glDeleteFramebuffers(1, &m_FboID);
    glDeleteTextures(m_TexIDs.size(), m_TexIDs.data());
    glDeleteTextures(m_DepthTexIDs.size(), m_DepthTexIDs.data());
}

void Framebuffer::createTextureChain(unsigned int &texId, const bool recreate, const GLenum attachment)
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

    glTextureStorage2D(texId, chainDepth, GL_RGB32F, currentSizeX, currentSizeY);

    if (recreate)
    {
        glNamedFramebufferTexture(m_FboID, attachment, texId, 0);
    }
}

void Framebuffer::createDepthTexture(unsigned int &depthTexID, const bool recreate)
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
    glBindImageTexture(unit, m_TexIDs[i], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
}

void Framebuffer::bindDepthTex(const int i) const
{
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

    if (m_DepthTexIDs.size() >= i)
    {
        this->bindDepthTex(i);
        createDepthTexture(m_DepthTexIDs[i], true);
    }
    
    if (m_TexIDs.size() >= i)
    {
        this->bindTex(i);
        createTextureChain(m_TexIDs[i], true);
    }

    this->unbind();
}
