#version 330

in vec3 fragPosition; 
in vec3 fragNormal;    

layout(location=0)out vec3 mrtPosition;
layout(location=1)out vec3 mrtNormal;

void main()
{
    mrtPosition = fragPosition;
    mrtNormal = fragNormal;
}