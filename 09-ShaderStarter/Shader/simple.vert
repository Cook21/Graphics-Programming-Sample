#version 130
//uniform mat4 gl_ModelViewMatrix;  
//uniform mat4 gl_ProjectionMatrix;
//in vec4 gl_Vertex; 


//��������
void main(void)
{
	gl_Position =gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//	gl_Position =ftransform();  
}


////////////////////////////////////////////////////////////
/*	
void main(void)
{
	vec4 v = gl_Vertex;		//ע��gl_Vertex��ֻ���ģ����Ա��븳����һ���������ܶԶ�����иı䡣
	v.x = v.x * 0.5;
	v.y = v.y * 0.5;
	gl_Position =gl_ProjectionMatrix * gl_ModelViewMatrix * v;
}
*/


/*	
void main(void)
{
	vec4 v = gl_Vertex;  
    v.z = 0.0;
	gl_Position =gl_ModelViewProjectionMatrix * v;
}
*/

/*	
void main(void)
{
	vec4 v = gl_Vertex;
    v.z = sin(5.0*v.x)*0.25;  
    gl_Position =gl_ModelViewProjectionMatrix * v;
}
*/

/*
uniform float time;
void main(void)  
{  
    vec4 v = gl_Vertex;  
    v.z = sin(5.0*v.x + time)*0.25;  
    gl_Position =gl_ModelViewProjectionMatrix * v;  
}
*/