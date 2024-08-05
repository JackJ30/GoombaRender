#include "uniform_settings.h"

namespace GoombaRender
{
    void UniformSettings::SetUniforms(ShaderInfo& shader) const
    {
        for (auto& uniform : bools)
        {
            shader.SetUniformBool(uniform.first, uniform.second);
        }
        for (auto& uniform : ints)
        {
            shader.SetUniformInt(uniform.first, uniform.second);
        }
        for (auto& uniform : floats)
        {
            shader.SetUniformFloat(uniform.first, uniform.second);
        }
        for (auto& uniform : vec2s)
        {
            shader.SetUniformVec2(uniform.first, uniform.second);
        }
        for (auto& uniform : vec3s)
        {
            shader.SetUniformVec3(uniform.first, uniform.second);
        }
        for (auto& uniform : vec4s)
        {
            shader.SetUniformVec4(uniform.first, uniform.second);
        }
        for (auto& uniform : mat2s)
        {
            shader.SetUniformMat2(uniform.first, uniform.second);
        }
        for (auto& uniform : mat3s)
        {
            shader.SetUniformMat3(uniform.first, uniform.second);
        }
        for (auto& uniform : mat4s)
        {
            shader.SetUniformMat4(uniform.first, uniform.second);
        }
    }
} // GoombaRender