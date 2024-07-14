#ifndef GOOMBARENDER_INDEX_BUFFER_H
#define GOOMBARENDER_INDEX_BUFFER_H

#include "renderer/ogl_obj.h"

namespace GoombaRender
{
    class IndexBuffer : public OglObj
    {
    public:
        IndexBuffer() = default;
        ~IndexBuffer();
        
        void Create(size_t count);
        void Create(unsigned int* indices, size_t count);
        
        void Bind() const;
        void Unbind() const;
        
        void SetData(unsigned int* indices, size_t count);
        
        inline size_t GetCount() const { return m_Count; }
    private:
        unsigned int m_RendererID;
        size_t m_Count;
    };

} // GoombaRender

#endif //GOOMBARENDER_INDEX_BUFFER_H
