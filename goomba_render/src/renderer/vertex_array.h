//
// Created by jack on 6/22/24.
//

#ifndef GOOMBARENDER_VERTEX_ARRAY_H
#define GOOMBARENDER_VERTEX_ARRAY_H

#include "renderer/vertex_buffer.h"
#include "renderer/index_buffer.h"

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
        
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer);
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer);
    private:
        unsigned int m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_VERTEX_ARRAY_H
