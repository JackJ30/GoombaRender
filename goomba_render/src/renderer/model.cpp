#include "model.h"

#include <glm/gtc/quaternion.hpp>

namespace GoombaRender
{
    void Model::Create()
    {
        RequireContext();
        m_Created = true;
    }
    
    void Model::Delete()
    {
        RequireContext();
        DEBUG_ASSERT(m_Created, "Model must be created before deleting.");
        
        for (auto& mesh : m_Meshes)
        {
            mesh.vao.Delete();
        }
        m_Context.GetGlad().DeleteBuffers(m_Buffers.size(), m_Buffers.data());
    }
    
    void Model::AddMesh(const Mesh& mesh)
    {
        m_Meshes.push_back(mesh); // TODO - move this
    }
    
    void LoadModel(Asset<Model> &asset, GoombaEngine::GraphicsContext &context)
    {
        if (asset.TryUseCached()) return;
        
        tinygltf::TinyGLTF loader;
        std::string error;
        std::string warn;
        
        tinygltf::Model loadedGLTF;
        bool ret = false;
        if (asset.GetPath().extension() == ".gltf")
        {
            ret = loader.LoadASCIIFromFile(&loadedGLTF, &error, &warn, asset.GetPath().string());
        }
        else if (asset.GetPath().extension() == ".glb")
        {
            ret = loader.LoadBinaryFromFile(&loadedGLTF, &error, &warn, asset.GetPath().string());
        }
        else { GLogError("Model must have glb or gltf file extension."); return; }
        
        if (!warn.empty()) {
            GLogError("GLTF loading warning for file '{}' :\n{}", asset.GetPath().string(), warn);
        }
        if (!error.empty()) {
            GLogError("GLTF loading error for file '{}' :\n{}", asset.GetPath().string(), error);
            return;
        }
        
        if (!ret) { GLogCritical("Could not load GLTF file with path '{}'", asset.GetPath().string()); return; }
        
        Model model;
        model.AssignContext(context);
        model.Create();
        
        // --- Converting tinyGLTF to GoombaRender model ---
        
        // Create buffers
        model.GetBuffers().reserve(loadedGLTF.buffers.size());
        context.GetGlad().GenBuffers(loadedGLTF.buffers.size(), model.GetBuffers().data());
        for (size_t i = 0; i < loadedGLTF.buffers.size(); ++i)
        {
            context.GetGlad().BindBuffer(GL_ARRAY_BUFFER, model.GetBuffers()[i]);
            context.GetGlad().BufferData(GL_ARRAY_BUFFER, loadedGLTF.buffers[i].data.size(), loadedGLTF.buffers[i].data.data(), GL_STATIC_DRAW);
        }
        
        // Helper lambda functions
        const std::function<void(const tinygltf::Node&, glm::mat4)> traverseNode = [&](const tinygltf::Node& node, glm::mat4 parentTransform)
        {
            // Deal with node
            
            // transformation
            glm::mat4 localTransform = parentTransform;
            if (!node.matrix.empty())
            {
                localTransform *= glm::mat4(node.matrix[0], node.matrix[1],
                                            node.matrix[2], node.matrix[3], node.matrix[4],
                                            node.matrix[5], node.matrix[6], node.matrix[7],
                                            node.matrix[8], node.matrix[9], node.matrix[10],
                                            node.matrix[11], node.matrix[12], node.matrix[13],
                                            node.matrix[14], node.matrix[15]);
            }
            else
            {
                if (!node.scale.empty()) { localTransform = glm::scale(localTransform, {node.scale[0], node.scale[1], node.scale[2]} ); }
                if (!node.rotation.empty()) { localTransform *= glm::mat4_cast(glm::quat(float(node.rotation[0]), float(node.rotation[1]), float(node.rotation[2]), float(node.rotation[3]))); }
                if (!node.translation.empty()) { localTransform = glm::translate(localTransform, {node.translation[0], node.translation[1], node.translation[2]} ); }
            }
            
            if (node.mesh >= 0)
            {
                for (tinygltf::Primitive primitive : loadedGLTF.meshes[node.mesh].primitives)
                {
                    // deal with mesh (primitive)
                    Mesh mesh;
                    mesh.vao.AssignContext(context);
                    mesh.vao.Create(primitive.indices >= 0 ? DrawType::Indices : DrawType::Arrays, primitive.mode);
                    mesh.localTransform = localTransform;
                    
                    // go through attributes
                    std::string attributes[3] = {"POSITION", "NORMAL", "TEXCOORD_0"};
                    unsigned int attribute_indexes[3] = {0, 1, 2};
                    
                    for (int i = 0; i < 3; ++i)
                    {
                        if (primitive.attributes.count(attributes[i]) > 0)
                        {
                            const auto accessorIdx = primitive.attributes[attributes[i]];
                            const auto &accessor = loadedGLTF.accessors[accessorIdx];
                            const auto &bufferView = loadedGLTF.bufferViews[accessor.bufferView];
                            const auto bufferIdx = bufferView.buffer;
                            
                            DEBUG_ASSERT(GL_ARRAY_BUFFER == bufferView.target, "It's gotta be an array buffer");
                            mesh.vao.BindAttribute(model.GetBuffers()[bufferIdx], attribute_indexes[i],
                                                   accessor.type, accessor.componentType,
                                                   GL_FALSE, bufferView.byteStride,
                                                   accessor.byteOffset + bufferView.byteOffset);
                        }
                    }
                    
                    if (primitive.indices >= 0)
                    {
                        const auto accessorIdx = primitive.indices;
                        const auto &accessor = loadedGLTF.accessors[accessorIdx];
                        const auto &bufferView = loadedGLTF.bufferViews[accessor.bufferView];
                        const auto bufferIdx = bufferView.buffer;
                        
                        unsigned int count = accessor.count;
                        unsigned int type = accessor.componentType;
                        
                        IndicesSection section = {accessor.byteOffset + bufferView.byteOffset, count, type};
                        mesh.vao.SetIndexBuffer(model.GetBuffers()[bufferIdx], { section });
                        
                        DEBUG_ASSERT(GL_ELEMENT_ARRAY_BUFFER == bufferView.target, "It's gotta be an index buffer");
                    }
                    
                    model.AddMesh(mesh);
                }
            }
            
            // Traverse children recursively
            for (auto nodeIDX : node.children)
            {
                traverseNode(loadedGLTF.nodes[nodeIDX], localTransform);
            }
        };
        
        // Start going through all scenes and their nodes
        for (auto scene : loadedGLTF.scenes)
        {
            for (auto nodeIDX : scene.nodes)
            {
                traverseNode(loadedGLTF.nodes[nodeIDX], glm::mat4(1.0));
            }
        }
        
        asset.AssignLoaded(std::move(model));
    }
} // GoombaRender