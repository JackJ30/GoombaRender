#ifndef GOOMBARENDER_TEXTURE_H
#define GOOMBARENDER_TEXTURE_H

#include "renderer/asset.h"

// TODO - Support mipmapping, and mipmapping filters
// TODO - Support other internal formats

namespace GoombaRender
{
    enum TextureFilterType
    {
        Linear, Nearest
    };
    
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        
        virtual void SetFiltering(TextureFilterType minFilter, TextureFilterType magFilter) = 0;
        
        virtual void Bind(unsigned int unit = 0) = 0;
        virtual void Unbind() = 0;
    };
    
    class Texture2D : public Texture
    {
    public:
        Texture2D();
        
        void Create(const unsigned char* data, int width, int height, GLenum format, GLenum dataType);
        void Delete();
        
        inline virtual unsigned int GetWidth() const override { return m_Width; };
        inline virtual unsigned int GetHeight() const override { return m_Height; }
        
        void SetFiltering(TextureFilterType minFilter, TextureFilterType magFilter) override;
        void SetWrapping(GLint wrapS, GLint wrapT);
        
        void Bind(unsigned int unit = 0) override;
        void Unbind() override;
    private:
        unsigned int m_RendererID;
        unsigned int m_BoundUnit = 0;
        
        unsigned int m_Width, m_Height;
        TextureFilterType m_MinFilter, m_MagFilter = TextureFilterType::Linear;
        GLint m_WrapS, m_WrapT = GL_REPEAT;
    };
    
    void LoadTexture2D(Asset<Texture2D>& asset, GoombaEngine::GraphicsContext& context);
    
} // GoombaRender

#endif //GOOMBARENDER_TEXTURE_H
