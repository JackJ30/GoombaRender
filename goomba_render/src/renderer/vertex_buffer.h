#ifndef GOOMBARENDER_VERTEX_BUFFER_H
#define GOOMBARENDER_VERTEX_BUFFER_H


namespace GoombaRender
{
    class VertexBuffer
    {
    public:
        VertexBuffer(unsigned int size);
        VertexBuffer(float* vertices, size_t size);
        virtual ~VertexBuffer();
        
        void Bind() const;
        void Unbind() const;
        
        virtual void SetData(const void* data, size_t size) override;
        
        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

} // GoombaRender

#endif //GOOMBARENDER_VERTEX_BUFFER_H
