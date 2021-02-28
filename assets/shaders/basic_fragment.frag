#version 330 core

out vec4 color;

in vec3 normalDir;
in vec2 texCoords;
in vec3 fragPos;
in mat3 NormalMatrix;
in vec3 lightDir;

uniform vec3 LightColor;
uniform mat4 ViewMatrix;
uniform float LightShiness;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main()
{
	vec3 baseColor = texture(diffuseTexture, texCoords).rgb;
	vec3 specularValue = texture(specularTexture, texCoords).rgb;

	vec3 normal = normalize(vec3(NormalMatrix * normalDir));
	vec3 viewDir = normalize(fragPos - vec3(0,0,0));

	vec3 ambientColor = 0.05f * LightColor;

	float diffuse = max(dot(normal, lightDir), 0.0f);
	vec3 diffuseColor = diffuse * LightColor;
	
	vec3 specularColor = vec3(0, 0, 0);

	if(diffuse > 0.0f)
	{
		vec3 halfVector = normalize(-viewDir + -lightDir);
		float specular = pow(max(dot(normal, halfVector), 0.0f), LightShiness);
		specularColor = specularValue * specular * LightColor;
	}
	
	color = vec4((ambientColor + diffuseColor + specularColor) * baseColor, 1.0f);
}