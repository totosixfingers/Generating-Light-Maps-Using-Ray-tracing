#version 330

in vec3 Pos; 
in vec3 N;    
in vec2 UV;

layout(location=0)out vec4 object1Coords;
layout(location=1)out vec4 object1Norms;
layout(location=2)out vec4 object1Texcoords;

void main()
{
    object1Coords = vec4(Pos, 1.0);
    object1Norms = vec4(N, 1.0);
	object1Texcoords=vec4(UV,0, 1.0);
}