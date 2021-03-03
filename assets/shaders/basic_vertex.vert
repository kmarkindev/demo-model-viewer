#version 330 core

layout(location = 0) in vec3 verticle;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec3 tangent;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 normalDir;
out vec2 texCoords;
out vec3 fragPos;
out mat3 NormalMatrix;
out mat3 TBN;

void main()
{
	texCoords = textureCoords;

	NormalMatrix = mat3(transpose(inverse(ModelMatrix)));
	normalDir = normalize(vec3(NormalMatrix * normal));

	// Gram-Schmidt process
	vec3 OptimizedTangent = normalize(tangent - dot(tangent, normalDir) * normalDir);

	vec3 bitTangent = cross(normalDir, tangent);
	TBN = mat3(OptimizedTangent, bitTangent, normalDir);

	vec4 vertPosition = ModelMatrix * vec4(verticle.xyz, 1.0f);

	fragPos = vertPosition.xyz;
	
	gl_Position = ProjectionMatrix * ViewMatrix * vertPosition;
}