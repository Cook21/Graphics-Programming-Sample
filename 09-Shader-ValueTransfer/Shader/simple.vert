#version 130
//uniform mat4 gl_ModelViewMatrix;  
//uniform mat4 gl_ProjectionMatrix;
//in vec4 gl_Vertex;
//out vec4 gl_FrontColor;
//out vec4 gl_Position;

uniform float time;
in vec4 vertCol;
out vec4 col;

void main(void)
{
    vec4 v = gl_Vertex;
    v.y += time * 0.1;  
	col = vertCol;
//	gl_FrontColor = vertCol;
    gl_Position =gl_ModelViewProjectionMatrix * v;
}