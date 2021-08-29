#version 130
in vec4 diffuse, ambient, specular, globalAmbient;
in vec3 normal, lightDir, eyeDir;

void main (void)
{
	vec3 normalizedLightDir = normalize(lightDir);
	vec3 normalizedEyeDir = normalize(eyeDir);
	vec3 normalizedNormal = normalize(normal);
  
	//����NdotL
	float NdotL = dot(normalizedNormal, normalizedLightDir);

	//����NdotH
	vec3 H = normalize(normalizedEyeDir + normalizedLightDir);
	float NdotH = dot(normalizedNormal, H);
	if(NdotL > 0.0)
		NdotH = max(NdotH, 0.0);
	else
		NdotH = 0.0;
	NdotL = max(NdotL, 0.0);

	//���������ɫ
	vec4 col = globalAmbient + ambient + diffuse * NdotL + specular * pow(NdotH, gl_FrontMaterial.shininess);

	gl_FragColor = col;
}