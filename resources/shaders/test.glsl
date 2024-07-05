#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec4 v_Color;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
    v_Color = a_Color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;
in vec4 v_Color;

void main()
{
    fragColor = v_Color;
}