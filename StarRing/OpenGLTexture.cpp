#include "pch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include "Log.h"

StarRing::OpenGLTexture2D::OpenGLTexture2D(string path):
    m_Path(path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc*data = stbi_load(path.c_str(),&width,&height,&channels,0);
    assert(data, "STBIMG导入失败！");
    m_Width = width;
    m_Height = height;

    GLenum internalFormat = 0,dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    assert(internalFormat & dataFormat,"图片格式不支持！");

    glCreateTextures(GL_TEXTURE_2D,1,&m_RendererID);
    glTextureStorage2D(m_RendererID,1, internalFormat,m_Width,m_Height);

    glTextureParameteri(m_RendererID,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_RendererID,0,0,0,m_Width,m_Height, dataFormat,GL_UNSIGNED_BYTE,data);

    stbi_image_free(data);
    SR_CORE_INFO("图片导入成功:{0}", path.c_str());
}

StarRing::OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

uint32_t StarRing::OpenGLTexture2D::GetHandle() const
{
    return m_RendererID;
}

void StarRing::OpenGLTexture2D::Bind(uint32_t tex_unit) const
{
    glActiveTexture(GL_TEXTURE0+tex_unit);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void StarRing::OpenGLTexture2D::UnBind(uint32_t tex_unit) const
{
    glActiveTexture(GL_TEXTURE0+tex_unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

StarRing::OpenGLTextureCubic::OpenGLTextureCubic(const vector<string>& path)
    :m_Path(path)
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

    int nrChannels = 0;
    m_Width = m_Height = 0;
    for (unsigned int i = 0; i < m_Path.size(); i++)
    {
        unsigned char* data = stbi_load(m_Path[i].c_str(), &m_Width, &m_Height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
            cout << "成功导入天空盒纹理" << endl;
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << m_Path[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

StarRing::OpenGLTextureCubic::~OpenGLTextureCubic()
{
    glDeleteTextures(1, &m_RendererID);
}

uint32_t StarRing::OpenGLTextureCubic::GetHandle() const
{
    return m_RendererID;
}

void StarRing::OpenGLTextureCubic::Bind(uint32_t tex_unit) const
{
    glActiveTexture(GL_TEXTURE0+tex_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void StarRing::OpenGLTextureCubic::UnBind(uint32_t tex_unit) const
{
    glActiveTexture(GL_TEXTURE0+tex_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
