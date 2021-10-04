#version 330 core

out vec3 Pos;
out vec3 N;
out vec2 UV;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    Pos = (model* vec4(aPos, 1.0)).xyz; 
	
     mat3 normalMatrix = transpose(inverse(mat3(model)));
    N = normalMatrix * Normal;

    gl_Position = projection*vec4(tex,0, 1.0);
}
