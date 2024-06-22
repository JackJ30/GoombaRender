#ifndef GOOMBARENDER_VERTEX_BUFFER_H
#define GOOMBARENDER_VERTEX_BUFFER_H

#include "goombapch.h"

namespace GoombaRender
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const std::vector<float>& vertices);
        ~VertexBuffer();
    private:
        void Bind();
        void Unbind();
        
    };

} // GoombaRender

#endif //GOOMBARENDER_VERTEX_BUFFER_H
