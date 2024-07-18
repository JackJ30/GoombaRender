#include "renderer.h"

namespace GoombaRender
{
    Renderer::Renderer(GoombaEngine::GraphicsContext &context)
        : m_Context(context)
    {
    
    }
    
    void Renderer::AddScenePass(const Camera& camera, const Scene& scene)
    {
        RenderPass pass;
        
        for (const Asset<Model>& model : scene.m_Objects)
        {
            for (const Mesh& mesh : model.Get().GetMeshes())
            {
                // temp uniforms
                UniformSetting setting;
                setting.mat4s.emplace_back("u_Transform", mesh.localTransform);
                setting.mat4s.emplace_back("u_View", glm::mat4(camera.GetViewMatrix()));
                setting.mat4s.emplace_back("u_Projection", camera.GetProjectionMatrix());
                
                pass.queue.push({mesh.vao, scene.m_MainShader, std::vector<std::pair<Asset<Texture2D>, unsigned int>>(), setting});
            }
        }
        
        m_RenderQueue.push(pass);
    }
    
    void Renderer::Render() // TODO - this function should be very optimized, and inputs should be sorted or something
    {
        for (; !m_RenderQueue.empty(); m_RenderQueue.pop()) // go through passes
        {
            RenderPass& pass = m_RenderQueue.front();
            
            for (; !pass.queue.empty(); pass.queue.pop()) // go through instructions
            {
                const RenderInstruction& instruction = pass.queue.front();
                
                instruction.vao.Bind();
                
                Shader& shader = instruction.shader.Get();
                shader.Bind();
                shader.SetUniforms(instruction.uniformSetting);
                
                for (auto& texture : instruction.textures)
                {
                    texture.first.Get().Bind(texture.second);
                }
                
                // Draw based on type
                if (instruction.vao.GetDrawType() == DrawType::Arrays)
                {
                    m_Context.GetGlad().DrawArrays(GL_TRIANGLES, 0, instruction.vao.GetNumVertices());
                }
                else if (instruction.vao.GetDrawType() == DrawType::Indices)
                {
                    for (auto& selection : instruction.vao.GetIndicesSections())
                    {
                        m_Context.GetGlad().DrawElements(GL_TRIANGLES, selection.count, selection.type, (const void*)selection.offset);
                    }
                }
            }
        }
    }
    
} // GoombaRender