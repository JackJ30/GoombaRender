#ifndef GOOMBARENDER_TEXTURE_H
#define GOOMBARENDER_TEXTURE_H

#include "renderer/graphics_context.h"

// TODO - Support mipmapping
// TODO - Support other internal formats

namespace GoombaRender
{
    struct Texture2DInfo
    {
    public:
        Texture2DInfo(unsigned int rendererID);
        
        Texture2DInfo(const Texture2DInfo&) = delete;
        Texture2DInfo& operator=(const Texture2DInfo&) = delete;
        Texture2DInfo(Texture2DInfo&&) = default;
        Texture2DInfo& operator=(Texture2DInfo&&) = default;
        
        void Delete();
        
        void Upload(const unsigned char* data, int width, int height, GLenum format, GLenum dataType);
        void Bind(unsigned int unit = 0) const;
        
        void SetFiltering(GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
        void SetWrapping(GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT);
        
        inline unsigned int GetRendererID() const { return rendererID; }
        inline unsigned int GetWidth() const { return width; };
        inline unsigned int GetHeight() const { return height; }
        
        unsigned int width, height;
    private:
        unsigned int rendererID;
    };
    
    Texture2DInfo CreateTexture2D(const unsigned char* data, int width, int height, GLenum format = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE, GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT);
    
} // GoombaRender

#endif //GOOMBARENDER_TEXTURE_H
