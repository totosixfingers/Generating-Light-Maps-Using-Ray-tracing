#version 330 core
layout(location = 0) out vec4 FragColor;

uniform sampler2D Sampler;
uniform vec2 invScreenSize;

in vec2 T;
#define OFFSET 1.0

void main()
{
	vec4 c = texture(Sampler, T);
	//if (c.a == 1){
		FragColor=texture(Sampler, T);
	//} else{
		//FragColor = vec4(255, 0, 0,1);
		//}
	
	
}
