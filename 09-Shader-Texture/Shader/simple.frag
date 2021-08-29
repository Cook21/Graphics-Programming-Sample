#version 130
in vec4 diffuse, ambient, specular, globalAmbient;
in vec3 normal, lightDir, eyeDir;
uniform sampler2D brickTex;  
uniform sampler2D textTex;  

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

	//����������ɫ
	vec4 texel = texture2D(brickTex,gl_TexCoord[0].st) * texture2D(textTex,gl_TexCoord[1].st);  
//	vec4 texel = texture2D(textTex,gl_TexCoord[1].st);  
	gl_FragColor = col * texel;
}