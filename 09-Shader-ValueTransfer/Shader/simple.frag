#version 130
//out vec4 gl_FragColor;
//in vec4 gl_Color;
in vec4 col; //����Զ��������������Ϊ��ʾ��ʵ�������ڽ�����gl_Color

void main (void)
{
//	gl_FragColor = vec4 (0.0, 1.0, 0.0, 1.0);
//	gl_FragColor = gl_Color;
	gl_FragColor = col;
}