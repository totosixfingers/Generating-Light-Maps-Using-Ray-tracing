#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
out vec2 uv;
out vec4 clipSpace;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    uv=vec2(UV);	
	clipSpace = projection * view * model * vec4(aPos, 1.0);
    gl_Position = clipSpace;
}