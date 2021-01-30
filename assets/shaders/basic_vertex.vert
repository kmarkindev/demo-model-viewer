#version 330 core

layout(location = 0) in vec3 verticle;
layout(location = 1) in vec3 normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 normalDir;

void main()
{
	normalDir = normal;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(verticle.xyz, 1.0f);
}