#version 330 core

out vec4 color;

in vec3 normalDir;
in vec2 texCoords;
in vec3 fragPos;
in mat3 NormalMatrix;
in mat3 TBN;

uniform vec3 LightColor;
uniform vec3 LightDir;
uniform float LightShiness;
uniform vec3 CameraPos;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D opacityTexture;
uniform sampler2D normalTexture;

uniform bool UseOpacity;
uniform bool UseNormal;

void main()
{
	vec3 objectColor = texture(diffuseTexture, texCoords).rgb;
	vec3 lightDir = -LightDir;

	vec3 norm = normalDir;
	if(UseNormal)
	{
		norm = texture(normalTexture, texCoords).rgb;
		norm = norm * 2.0 - 1.0;
		norm = normalize(TBN * norm);
	}

	// ambient
    vec3 ambient = 0.1 * LightColor;
  	
    // diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;
    
    // specular
	vec3 specular = vec3(0,0,0);

	if(diff > 0)
	{
		vec3 specularStrength = texture(specularTexture, texCoords).rgb;
		vec3 viewDir = normalize(CameraPos - fragPos);
		vec3 halfVector = normalize(viewDir + lightDir);
		float spec = pow(max(dot(norm, halfVector), 0.0), 32);
		specular = specularStrength * spec * LightColor;
	}
	
	float opacity = 1.f;

	if(UseOpacity)
	{
		opacity = texture(opacityTexture, texCoords).x;
	}

	color = vec4((ambient + diffuse + specular) * objectColor, opacity);
}