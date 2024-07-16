#include "texture.h"

#include <glad/gl.h>

namespace GoombaRender
{
    GLint GetGLFilter(TextureFilterType filter) {
        if (filter == TextureFilterType::Linear) return GL_LINEAR;
        else                                     return GL_NEAREST;
    }
    
    Texture2D::Texture2D()
        : m_MinFilter(TextureFilterType::Linear), m_MagFilter(TextureFilterType::Linear)
    {
    
    }
    
    void Texture2D::Create(const unsigned char* data, int width, int height, int channels)
    {
        RequireContext();
        
        m_Width = width;
        m_Height = height;
        
        m_Context.GetGlad().GenTextures(1, &m_RendererID);
        m_Context.GetGlad().BindTexture(GL_TEXTURE_2D, m_RendererID);
        m_Context.GetGlad().TexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_Width, m_Height);
        
        m_Context.GetGlad().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilter(m_MinFilter));
        m_Context.GetGlad().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilter(m_MagFilter));
        
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
        m_Context.GetGlad().TexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);
        
        m_Created = true;
    }
    
    void Texture2D::SetFiltering(TextureFilterType minFilter, TextureFilterType magFilter)
    {
        m_MinFilter = minFilter;
        m_MagFilter = magFilter;
        
        if (m_HasContext && m_Created)
        {
            m_Context.GetGlad().BindTexture(GL_TEXTURE_2D, m_RendererID);
            m_Context.GetGlad().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilter(m_MinFilter));
            m_Context.GetGlad().TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilter(m_MagFilter));
        }
    }
    
    void Texture2D::Bind(unsigned int unit)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Texture must be created before binding.");
        
        m_BoundUnit = unit;
        m_Context.GetGlad().ActiveTexture(GL_TEXTURE0 + m_BoundUnit);
        m_Context.GetGlad().BindTexture(GL_TEXTURE_2D, m_RendererID);
    }
    
    void Texture2D::Unbind()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Texture must be created before unbinding.");
        
        m_Context.GetGlad().ActiveTexture(GL_TEXTURE0 + m_BoundUnit);
        m_Context.GetGlad().BindTexture(GL_TEXTURE_2D, 0);
    }
    
    void Texture2D::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Texture must be created before deleting.");
        
        m_Context.GetGlad().DeleteTextures(1, &m_RendererID);
    }
} // GoombaRender