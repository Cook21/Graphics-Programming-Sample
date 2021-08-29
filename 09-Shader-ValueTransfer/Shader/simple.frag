#version 130
//out vec4 gl_FragColor;
//in vec4 gl_Color;
in vec4 col; //这个自定义输入变量仅作为演示，实际上有内建变量gl_Color

void main (void)
{
//	gl_FragColor = vec4 (0.0, 1.0, 0.0, 1.0);
//	gl_FragColor = gl_Color;
	gl_FragColor = col;
}