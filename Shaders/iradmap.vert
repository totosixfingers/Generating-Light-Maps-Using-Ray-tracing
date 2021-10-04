#version 330

uniform mat4 pvmMatrix;
in vec3 position;
in vec3 normal;

out vec3 fragPosition; 
out vec3 fragNormal;        

void main()
{
    fragPosition = position; 
    fragNormal = normal; 
    gl_Position = pvmMatrix * vec4(position, 1.0);
}