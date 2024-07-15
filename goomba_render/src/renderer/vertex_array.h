//
// Created by jack on 6/22/24.
//

#ifndef GOOMBARENDER_VERTEX_ARRAY_H
#define GOOMBARENDER_VERTEX_ARRAY_H

#include "renderer/ogl_obj.h"
#include "renderer/buffer_layout.h"

// TODO - support dynamic and stream draw

namespace GoombaRender
{
    
    class VertexArray : public OglObj
    {
    public:
        VertexArray() = default;
        ~VertexArray();
        
        void Create();
        
        void Bind() const;
        void Unbind() const;
        
        void AddVertexBuffer(float* vertices, size_t size, const BufferLayout& layout);
        void SetIndexBuffer(unsigned int *indices, size_t numIndices);
        
        inline unsigned int GetNumIndices() const { return m_NumIndices; }
    private:
        unsigned int m_RendererID;
        std::vector<unsigned int> m_VertexBuffers;
        unsigned int m_IndexBuffer;
        size_t m_NumIndices = 0;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_VERTEX_ARRAY_H
