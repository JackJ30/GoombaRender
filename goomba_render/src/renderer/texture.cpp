#include "texture.h"

#include <glad/gl.h>
#include <stb/stb_image.h>

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
    
    void Texture2D::Create(const unsigned char* data, int width, int height, GLenum format, GLenum dataType)
    {
        RequireContext();
        
        m_Width = width;
        m_Height = height;
        
        glad.GenTextures(1, &m_RendererID);
        glad.BindTexture(GL_TEXTURE_2D, m_RendererID);
        glad.TexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_Width, m_Height);
        
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilter(m_MinFilter));
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilter(m_MagFilter));
        
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapT);
        
        glad.TexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, format, dataType, data);
        
        m_Created = true;
    }
    
    void Texture2D::SetFiltering(TextureFilterType minFilter, TextureFilterType magFilter)
    {
        m_MinFilter = minFilter;
        m_MagFilter = magFilter;
        
        if (m_HasContext && m_Created)
        {
            glad.BindTexture(GL_TEXTURE_2D, m_RendererID);
            glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilter(m_MinFilter));
            glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilter(m_MagFilter));
        }
    }
    
    void Texture2D::SetWrapping(GLint wrapS, GLint wrapT)
    {
        m_WrapS = wrapS;
        m_WrapT = wrapT;
        
        if (m_HasContext && m_Created)
        {
            glad.BindTexture(GL_TEXTURE_2D, m_RendererID);
            glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
            glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapT);
        }
    }
    
    
    void Texture2D::Bind(unsigned int unit)
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Texture must be created before binding.");
        
        m_BoundUnit = unit;
        glad.ActiveTexture(GL_TEXTURE0 + m_BoundUnit);
        glad.BindTexture(GL_TEXTURE_2D, m_RendererID);
    }
    
    void Texture2D::Unbind()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Texture must be created before unbinding.");
        
        glad.ActiveTexture(GL_TEXTURE0 + m_BoundUnit);
        glad.BindTexture(GL_TEXTURE_2D, 0);
    }
    
    void Texture2D::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Texture must be created before deleting.");
        
        glad.DeleteTextures(1, &m_RendererID);
    }
    
    void LoadTexture2D(Asset<Texture2D>& asset, GoombaEngine::GraphicsContext& context)
    {
        if (asset.TryLoadFromCache()) return;
        if (!asset.GetPath().has_value()) { GLogError("Can not load texture with no path."); return; }
        
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(asset.GetPath().value().c_str(), &width, &height, &channels, 0);
        DEBUG_ASSERT(data != nullptr, fmt::format("Could not load image at path: '{}'", asset.GetPath().value()));
        DEBUG_ASSERT(channels == 3 || channels == 4, "Loaded images must have 3 or 4 channels.");
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
        
        Texture2D texture;
        texture.AssignContext(context);
        texture.Create(data, width, height, format, GL_UNSIGNED_BYTE);
        
        asset.AssignLoaded(std::move(texture));
        
        stbi_image_free(data);
    }
} // GoombaRender