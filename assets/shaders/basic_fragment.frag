#version 330

out vec4 color;

in vec3 normalDir;

uniform vec3 LightDir;

void main()
{
	vec3 lightDir = normalize(vec3(0.5f, 0.5f, 0.0f));
	vec4 baseColor = vec4(255.0f / 255.0f, 127.0f / 255.0f, 80.0f / 255.0f, 1.0f);

	float opacity = clamp(0.0f, 1.0f, dot(normalDir, lightDir) + 0.15f);

	color = opacity * baseColor;
}