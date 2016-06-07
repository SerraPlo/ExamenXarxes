//The fragment shader operates on each pixel in a given polygon
#version 330 core
 
layout(location=0) in vec3 vertPosition;
layout(location=1) in vec3 vertColor;
layout(location=2) in vec2 vertUV;

out vec3 fragColor;
out vec2 fragUV;
 
// Model, View, Projection matrix
uniform mat4 camera;
uniform mat4 model;
 
void main()
{
    gl_Position = camera*model*vec4(vertPosition, 1.0f);
	fragColor = vertColor;
    fragUV = vec2(vertUV.x, 1.0 - vertUV.y);
}