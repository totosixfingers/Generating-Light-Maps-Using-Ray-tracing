#version 330 core
out vec4 FragColor;


in vec2 uv;
uniform sampler2D iradtexture;

void main() {

	//vec2 ndc = (uv.xy)/2.0 +0.5;
	//vec2 texcoords = vec2(ndc.x, ndc.y);
	FragColor = texture(iradtexture, uv);
}