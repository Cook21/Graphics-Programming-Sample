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
    vec4 ambient;		//全局的环境光参数
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

out vec4 diffuse, ambient, specular, globalAmbient;
out vec3 normal, lightDir, eyeDir;

void main(void)
{
	//传递纹理坐标
	gl_TexCoord[0]  = gl_MultiTexCoord0;
	gl_TexCoord[1]  = gl_TextureMatrix[1] * gl_MultiTexCoord1; //实现文字移动

	//计算光照方向
	vec4 v = gl_ModelViewMatrix * gl_Vertex;
    lightDir = vec3(gl_LightSource[0].position - v);
	lightDir = normalize(lightDir);

	//计算物体的法向
	normal = normalize(vec3(gl_NormalMatrix * gl_Normal));  
	
	//计算视线方向
//	eyeDir = vec3((vec4(0.0) - v));
	eyeDir = vec3(-v);
	eyeDir = normalize(eyeDir);

	//计算ambient分量
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_FrontMaterial.ambient * gl_LightModel.ambient;  //这是全局的环境光
	
	//计算diffuse分量的不变部分 
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	//计算specular分量的不变部分
	specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;

    gl_Position = ftransform();
}