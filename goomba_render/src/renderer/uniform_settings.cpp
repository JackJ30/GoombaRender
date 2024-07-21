#include "uniform_settings.h"

namespace GoombaRender
{
    void Shader::SetUniforms(const UniformSetting &setting)
    {
        for (auto& uniform : setting.bools)
        {
            SetUniformBool(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.ints)
        {
            SetUniformInt(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.floats)
        {
            SetUniformFloat(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.vec2s)
        {
            SetUniformVec2(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.vec3s)
        {
            SetUniformVec3(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.vec4s)
        {
            SetUniformVec4(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.mat2s)
        {
            SetUniformMat2(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.mat3s)
        {
            SetUniformMat3(uniform.first, uniform.second);
        }
        for (auto& uniform : setting.mat4s)
        {
            SetUniformMat4(uniform.first, uniform.second);
        }
    }
} // GoombaRender