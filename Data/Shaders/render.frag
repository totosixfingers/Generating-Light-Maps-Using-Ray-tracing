#version 330 core
out vec4 FragColor;


in vec2 UV;
uniform vec3 uniform_diffuse;
uniform sampler2D iradtexture;
uniform float uniform_has_texture;
void main() {
//vec4 diffuseColor = vec4(uniform_diffuse.rgb, 1);
	//FragColor = mix( diffuseColor, diffuseColor * texture(iradtexture, UV), uniform_has_texture);
	FragColor= texture(iradtexture, UV);
}