#version 330 core
layout(location = 0) out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{		
	
    
    
    FragColor = vec4(WorldPos, 1.0);
}