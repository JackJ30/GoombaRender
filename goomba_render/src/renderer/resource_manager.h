#ifndef GOOMBARENDER_RESOURCE_MANAGER_H
#define GOOMBARENDER_RESOURCE_MANAGER_H

#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/model.h"

// TODO - loading shouldn't require shared pointers, should be able to load and have it copied, with some kind of cache

namespace GoombaRender
{
    std::shared_ptr<ShaderInfo> LoadShader(const std::string &path, bool cache = true);
    std::shared_ptr<Texture2DInfo> LoadTexture2D(const std::string& path, GLint minFilter,
                                              GLint magFilter, GLint wrapS, GLint wrapT, bool cache = true);
    void InitializeModelLoader(std::string pbrShaderPath);
    std::shared_ptr<Model> LoadModel(const std::string& path, bool cache = true);
} // GoombaRender

#endif //GOOMBARENDER_RESOURCE_MANAGER_H
