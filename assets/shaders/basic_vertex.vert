#version 330

layout(location = 0) in vec3 verticle;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 textureCoords;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;

out vec3 normalDir;

void main()
{
	normalDir = normal;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(verticle.xyz, 1);
}