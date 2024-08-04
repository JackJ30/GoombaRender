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
                mesh.material->GetUniformSettings().AssignUniformMat4("u_Transform", mesh.localTransform * object.transform.GetTransformationMatrix());
                mesh.material->GetUniformSettings().AssignUniformMat4("u_View", glm::mat4(camera.GetViewMatrix()));
                mesh.material->GetUniformSettings().AssignUniformMat4("u_Projection", camera.GetProjectionMatrix());
                
                pass.queue.emplace(mesh.vao, mesh.material);
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