#include <stdio.h>
#include <Windows.h>
#include <GL/freeglut.h>

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//��ʾģʽ��ʼ��
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//���崰�ڴ�С
	glutInitWindowSize(300, 300);
	//���崰��λ��
	glutInitWindowPosition(100, 100);
	//��������
	glutCreateWindow("testgl");
	const char* version = (const char*)glGetString(GL_VERSION);
	printf("OpenGL �汾��%s\n", version);
	glutMainLoop();  //����GLUT�¼�����ѭ��������ѭ��
	return 0;
}
