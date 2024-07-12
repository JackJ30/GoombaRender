#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_Transform; // Could pre-multiply, don't know if that is more effecient, I've heard gpus are good at matrix multiplcation
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_Color;

void main()
{
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
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