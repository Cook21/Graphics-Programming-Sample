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
  
	//计算NdotL
	float NdotL = dot(normalizedNormal, normalizedLightDir);

	//计算NdotH
	vec3 H = normalize(normalizedEyeDir + normalizedLightDir);
	float NdotH = dot(normalizedNormal, H);
	if(NdotL > 0.0)
		NdotH = max(NdotH, 0.0);
	else
		NdotH = 0.0;
	NdotL = max(NdotL, 0.0);

	//计算光照颜色
	vec4 col = globalAmbient + ambient + diffuse * NdotL + specular * pow(NdotH, gl_FrontMaterial.shininess);

	//计算纹理颜色
	vec4 texel = texture2D(brickTex,gl_TexCoord[0].st) * texture2D(textTex,gl_TexCoord[1].st);  
//	vec4 texel = texture2D(textTex,gl_TexCoord[1].st);  
	gl_FragColor = col * texel;
}