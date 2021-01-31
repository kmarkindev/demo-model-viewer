#version 330 core

out vec4 color;

in vec3 normalDir;

uniform vec3 LightDir;
uniform vec3 CameraDir;
uniform mat4 ModelMatrix;
uniform vec4 BaseColor;

void main()
{
	vec3 normal = normalize(vec3(mat3(transpose(inverse(ModelMatrix))) * normalDir));
	vec3 lightDir = normalize(LightDir);

	float diffuse = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(LightDir, normal);
	float spec = pow(max(dot(CameraDir, reflectDir), 0.0), 32);

	float specular = 0.5f * spec; 

	float ambient = 0.05f;

	color = (ambient + diffuse + specular) * BaseColor;
}