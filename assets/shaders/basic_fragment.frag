#version 330 core

out vec4 color;

in vec3 normalDir;
in vec2 texCoords;

uniform vec3 LightDir;
uniform vec3 LightColor;
uniform vec3 CameraDir;
uniform mat4 ModelMatrix;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main()
{
	vec3 baseColor = vec3(texture(diffuseTexture, texCoords));
	vec3 specularSense = vec3(texture(specularTexture, texCoords));

	vec3 normal = normalize(vec3(mat3(transpose(inverse(ModelMatrix))) * normalDir));
	vec3 lightDir = normalize(LightDir);

	float diffuseOpacity = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diffuseOpacity * LightColor * baseColor;

	vec3 reflectDir = reflect(LightDir, normal);
	float spec = pow(max(dot(CameraDir, reflectDir), 0.0), 32);
	vec3 specular = specularSense * LightColor * spec; 

	vec3 ambient = baseColor * 0.05f;

	color = vec4(ambient + diffuse + specular, 1.0f);
}