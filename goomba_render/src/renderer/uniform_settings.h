#ifndef GOOMBARENDER_UNIFORM_SETTINGS_H
#define GOOMBARENDER_UNIFORM_SETTINGS_H

namespace GoombaRender
{
    // TODO - uniform settings
    // Should take in a shader as reference. It should have functions for "trying" to set a uniform, it will only permit uniforms that the shader actually has. It will have a function to set all the uniforms of the shader
    // TODO - manage reference to the shader better, after the asset rework
    
    class UniformSettings
    {
        std::vector<std::pair<std::string, bool>> bools;
        std::vector<std::pair<std::string, int>> ints;
        std::vector<std::pair<std::string, float>> floats;
        std::vector<std::pair<std::string, glm::vec2>> vec2s;
        std::vector<std::pair<std::string, glm::vec3>> vec3s;
        std::vector<std::pair<std::string, glm::vec4>> vec4s;
        std::vector<std::pair<std::string, glm::mat2>> mat2s;
        std::vector<std::pair<std::string, glm::mat3>> mat3s;
        std::vector<std::pair<std::string, glm::mat4>> mat4s;
    };
    
} // GoombaRender

#endif //GOOMBARENDER_UNIFORM_SETTINGS_H
