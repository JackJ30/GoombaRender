#include "renderer.h"

namespace GoombaRender
{
    void Renderer::AddScenePass(const Camera& camera, const Scene& scene)
    {
        RenderPass pass;
        
        for (const SceneObject& object : scene.m_Objects)
        {
            for (const Mesh& mesh : object.model->GetMeshes())
            {
                // Set temp material uniforms (will be replaced with ubo and other system for "renderer uniform")
                if (mesh.material->HasUniform("u_Transform")) { mesh.material->AssignUniformMat4("u_Transform", mesh.localTransform * object.transform.GetTransformationMatrix()); }
                if (mesh.material->HasUniform("u_View")) { mesh.material->AssignUniformMat4("u_View", glm::mat4(camera.GetViewMatrix())); }
                if (mesh.material->HasUniform("u_Projection")) { mesh.material->AssignUniformMat4("u_Projection", camera.GetProjectionMatrix()); }
                
                pass.queue.emplace(*mesh.vao, *mesh.material);
            }
        }
        
        m_RenderQueue.push(pass);
    }
    
    void Renderer::Initialize()
    {
        glad.ClearColor(.1f, .2f, .3f, 1.0f);
    }
    
    void Renderer::Render() // TODO - this function should be very optimized, and inputs should be sorted or something
    {
        for (; !m_RenderQueue.empty(); m_RenderQueue.pop()) // go through passes
        {
            RenderPass& pass = m_RenderQueue.front();
            
            glad.Enable(GL_DEPTH_TEST);
            glad.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            for (; !pass.queue.empty(); pass.queue.pop()) // go through instructions
            {
                const RenderInstruction& instruction = pass.queue.front();
                
                instruction.vao.Bind();
                instruction.material.Bind();
                
                // Draw based on type
                if (instruction.vao.layoutType == LayoutType::Arrays)
                {
                    glad.DrawArrays(instruction.vao.drawMode, 0, instruction.vao.numVertices);
                }
                else if (instruction.vao.layoutType == LayoutType::Indices)
                {
                    for (auto& selection : instruction.vao.indicesInfo)
                    {
                        glad.DrawElements(instruction.vao.drawMode, selection.count, selection.type, (const void*)selection.offset);
                    }
                }
            }
        }
    }
} // GoombaRender