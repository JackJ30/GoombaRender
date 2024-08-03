#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Transform; // Could pre-multiply, don't know if that is more effecient, I've heard gpus are good at matrix multiplcation
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, v_TexCoord);
}