
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
out vec2 T;

uniform mat4 projection;


void main()
{
    T = UV;  
    gl_Position =projection *vec4(UV,0, 1.0);
}