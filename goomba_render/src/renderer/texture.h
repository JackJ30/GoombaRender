#ifndef GOOMBARENDER_TEXTURE_H
#define GOOMBARENDER_TEXTURE_H

#include "renderer/ogl_obj.h"

// TODO - Support mipmapping, and mipmapping filters
// TODO - Support creating with pure data, image loading should not be essential to creating a texture

namespace GoombaRender
{
    enum TextureFilterType
    {
        Linear, Nearest
    };
    
    class Texture : public OglObj
    {
    public:
        virtual ~Texture() = default;
        
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        
        virtual void SetFiltering(TextureFilterType minFilter, TextureFilterType magFilter) = 0;
        
        virtual void Bind(unsigned int unit = 0) const = 0;
    };
    
    class Texture2D : public Texture
    {
    public:
        Texture2D();
        ~Texture2D() override;
        
        void Create(const std::string& path);
        
        inline virtual unsigned int GetWidth() const override { return m_Width; };
        inline virtual unsigned int GetHeight() const override { return m_Height; }
        
        void SetFiltering(TextureFilterType minFilter, TextureFilterType magFilter) override;
        
        void Bind(unsigned int unit = 0) const override;
    private:
        unsigned int m_RendererID;
        bool m_Created;
        unsigned int m_Width, m_Height;
        TextureFilterType m_MinFilter, m_MagFilter;
    };
} // GoombaRender

#endif //GOOMBARENDER_TEXTURE_H
