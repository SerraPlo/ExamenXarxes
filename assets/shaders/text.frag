//The fragment shader operates on each pixel in a given polygon
#version 330 core

in vec2 fragColor;
in vec2 fragUV;

uniform sampler2D texture_sampler;
 
layout (location=0) out vec4 finalColor;

void main() {
	finalColor = texture(texture_sampler, fragUV);
}