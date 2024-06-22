#ifndef GOOMBARENDER_INDEX_BUFFER_H
#define GOOMBARENDER_INDEX_BUFFER_H

#include "goombapch.h"

namespace GoombaRender
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const std::vector<unsigned int>& indices);
        ~IndexBuffer();
    private:
        void Bind();
        void UnBind();
    };

} // GoombaRender

#endif //GOOMBARENDER_INDEX_BUFFER_H
