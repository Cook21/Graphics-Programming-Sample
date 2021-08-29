#version 130
//uniform mat4 gl_ModelViewMatrix;  
//uniform mat4 gl_ProjectionMatrix;
//in vec4 gl_Vertex;
//out vec4 gl_Position;
/*
struct gl_LightSourceParameters  
{  
    vec4 ambient;  
    vec4 diffuse;  
    vec4 specular;  
    vec4 position;  
    vec4 halfVector;  
    vec3 spotDirection;  
    float spotExponent;  
    float spotCutoff; // (range: [0.0,90.0], 180.0)   
    float spotCosCutoff; // (range: [1.0,0.0],-1.0)   
    float constantAttenuation;  
    float linearAttenuation;  
    float quadraticAttenuation;  
};  
uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];  

struct gl_LightModelParameters  
{  
    vec4 ambient;		//ȫ�ֵĻ��������
};  
uniform gl_LightModelParameters gl_LightModel;  

struct gl_MaterialParameters  
{  
    vec4 emission;  
    vec4 ambient;  
    vec4 diffuse;  
    vec4 specular;  
    float shininess;  
};  
uniform gl_MaterialParameters gl_FrontMaterial;  
uniform gl_MaterialParameters gl_BackMaterial;  
*/

//out vec4 col;

void main(void)
{
	//������շ���
	vec4 v = gl_ModelViewMatrix * gl_Vertex; 
    vec3 lightDir = vec3(gl_LightSource[0].position - v);
	lightDir = normalize(lightDir);

	//��������ķ���
	vec3 n = normalize(vec3(gl_NormalMatrix * gl_Normal));  
	
	//����H
	vec3 eyeDir = vec3((vec4(0.0) - v));
	eyeDir = normalize(eyeDir);
	vec3 H = normalize(eyeDir + lightDir);

	//����NdotL��NdotH
	float NdotL = dot(n, lightDir);
	float NdotH = dot(n, H);
	if(NdotL > 0.0)
		NdotH = max(NdotH, 0.0);
	else
		NdotH = 0.0;
	NdotL = max(NdotL, 0.0);

	//����ambient����
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 globalAmbient = gl_FrontMaterial.ambient * gl_LightModel.ambient;  //����ȫ�ֵĻ�����
	
	//����diffuse���� 
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL;

	//����specular����
	vec4 specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotH, gl_FrontMaterial.shininess);  

	gl_FrontColor = globalAmbient + ambient + diffuse + specular;
//	col = diffuse + specular;
    gl_Position = ftransform();
}