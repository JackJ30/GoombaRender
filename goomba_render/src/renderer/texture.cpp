#include "texture.h"

#include <glad/gl.h>

namespace GoombaRender
{
    Texture2DInfo::Texture2DInfo(unsigned int rendererID)
        : rendererID(rendererID)
    {
    
    }
    
    void Texture2DInfo::Upload(const unsigned char* data, int width, int height, GLenum format, GLenum dataType)
    {
        width = width;
        height = height;
        
        glad.GenTextures(1, &rendererID);
        glad.BindTexture(GL_TEXTURE_2D, rendererID);
        glad.TexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
        
        glad.TexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, dataType, data);
    }
    
    void Texture2DInfo::SetFiltering(GLint minFilter, GLint magFilter)
    {
        glad.BindTexture(GL_TEXTURE_2D, rendererID);
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }
    
    void Texture2DInfo::SetWrapping(GLint wrapS, GLint wrapT)
    {
        glad.BindTexture(GL_TEXTURE_2D, rendererID);
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glad.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapT);
    }
    
    void Texture2DInfo::Bind(unsigned int unit) const
    {
        glad.ActiveTexture(GL_TEXTURE0 + unit);
        glad.BindTexture(GL_TEXTURE_2D, rendererID);
    }
    
    void Texture2DInfo::Delete()
    {
        glad.DeleteTextures(1, &rendererID);
    }
    
    Texture2DInfo CreateTexture2D(const unsigned char *data, int width, int height, GLenum format, GLenum dataType, GLint minFilter,
                    GLint magFilter, GLint wrapS, GLint wrapT)
    {
        unsigned int id;
        glad.GenTextures(1, &id);
        Texture2DInfo texture(id);
        texture.SetFiltering(minFilter, magFilter);
        texture.SetWrapping(wrapS, wrapT);
        texture.Upload(data, width, height, format, dataType);
        return texture;
    }
} // GoombaRender