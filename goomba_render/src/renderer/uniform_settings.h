#ifndef GOOMBARENDER_UNIFORM_SETTINGS_H
#define GOOMBARENDER_UNIFORM_SETTINGS_H

#include "renderer/shader.h"

namespace GoombaRender
{
    struct UniformSettings
    {
    public:
        void SetUniforms(ShaderInfo& shader) const;
        
        std::vector<std::pair<std::string, bool>> bools;
        std::vector<std::pair<std::string, int>> ints;
        std::vector<std::pair<std::string, float>> floats;
        std::vector<std::pair<std::string, glm::vec2>> vec2s;
        std::vector<std::pair<std::string, glm::vec3>> vec3s;
        std::vector<std::pair<std::string, glm::vec4>> vec4s;
        std::vector<std::pair<std::string, glm::mat2>> mat2s;
        std::vector<std::pair<std::string, glm::mat3>> mat3s;
        std::vector<std::pair<std::string, glm::mat4>> mat4s;
        
        /**/
    };
    
} // GoombaRender

#endif //GOOMBARENDER_UNIFORM_SETTINGS_H
