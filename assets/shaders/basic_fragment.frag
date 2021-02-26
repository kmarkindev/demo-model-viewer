#version 330 core

out vec4 color;

in vec3 normalDir;
in vec2 texCoords;
in vec3 fragPos;
in mat3 NormalMatrix;

uniform vec3 LightDir;
uniform vec3 LightColor;
uniform mat4 ViewMatrix;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main()
{
	vec3 baseColor = texture(diffuseTexture, texCoords).rgb;
	vec3 specularValue = texture(specularTexture, texCoords).rgb;

	vec3 normal = normalize(vec3(NormalMatrix * normalDir));
	vec3 viewDir = normalize(fragPos - vec3(0,0,0));
	vec3 lightDir = normalize(vec3(ViewMatrix * vec4(normalize(LightDir), 0.f)));

	vec3 ambientColor = 0.05f * LightColor;

	float diffuse = max(dot(normal, lightDir), 0.0f);
	vec3 diffuseColor = diffuse * baseColor * LightColor;

	vec3 halfVector = normalize(-viewDir + -viewDir);
	float specular = pow(max(dot(normal, halfVector), 0.0f), 16.f);
	vec3 specularColor = specular * specularValue * LightColor;

	color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}