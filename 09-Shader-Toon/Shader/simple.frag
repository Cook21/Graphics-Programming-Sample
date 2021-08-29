#version 130
in vec3 normal, lightDir;

void main (void)
{
	vec3 normalizedLightDir = normalize(lightDir);
	vec3 normalizedNormal = normalize(normal);
  
	//¼ÆËãNdotL
	float NdotL = dot(normalizedNormal, normalizedLightDir);

	vec4 col;
	if (NdotL > 0.95)  
        col = vec4(1.0,0.5,0.5,1.0);  
    else if (NdotL > 0.5)  
        col = vec4(0.6,0.3,0.3,1.0);  
    else if (NdotL > 0.25)  
        col = vec4(0.4,0.2,0.2,1.0);  
    else  
        col = vec4(0.2,0.1,0.1,1.0);  

	gl_FragColor = col;
}