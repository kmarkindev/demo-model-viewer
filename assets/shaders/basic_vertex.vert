#version 330 core

layout(location = 0) in vec3 verticle;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform vec3 LightDir;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 normalDir;
out vec2 texCoords;
out vec3 fragPos;
out mat3 NormalMatrix;
out vec3 lightDir;

void main()
{
	normalDir = normal;
	texCoords = textureCoords;
	lightDir = normalize(vec3(ViewMatrix * vec4(normalize(LightDir), 0.f)));

	NormalMatrix = mat3(transpose(inverse(ViewMatrix * ModelMatrix)));

	vec4 vertPosition = ViewMatrix * ModelMatrix * vec4(verticle.xyz, 1.0f);

	fragPos = vertPosition.xyz;
	
	gl_Position = ProjectionMatrix * vertPosition;
}