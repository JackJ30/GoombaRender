#ifndef GOOMBARENDER_RESOURCE_MANAGER_H
#define GOOMBARENDER_RESOURCE_MANAGER_H

#include "renderer/shader.h"

// TODO - loading shouldn't require shared pointers, should be able to load and have it copied, with some kind of cache

namespace GoombaRender
{
    std::shared_ptr<ShaderInfo> LoadShader(const std::string &path, bool cache = true);
    
} // GoombaRender

#endif //GOOMBARENDER_RESOURCE_MANAGER_H
