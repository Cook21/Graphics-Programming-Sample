// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//��Ҫ������ͷ�ļ�
#include <stdio.h>
#include <windows.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/freeglut.h>

//����������ڵĴ�С
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

GLenum  G_shadingMode;	//�л���������ģʽ

//�����������ľ���
float G_fDistance = 3.6f;
//�������ת�Ƕ� 
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

////////////////////////////////////////////////
//���ղ���
float G_vLit0Position[4] = { 5.0f, 0.0f, 5.0f, 1.0f };
float G_vLit0Ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float G_vMaterialSpecu[4] = { 0.8f, 0.8f, 0.8f, 1.0f };

float G_vLit1Position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//��Ӧ��������, �Ӷ��趨�����ƽ���Զ�Լ���ת�Ļص�����
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);


////////////////////////////////////////////////
//������
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	//��ʼ��OPENGL��ʾ��ʽ
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);

	//�趨OPENGL����λ�úʹ�С
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
		
	//�򿪴���
	glutCreateWindow ("OpenGL");
	printf("�ϡ��¼�---��x����ת\n");
	printf("���Ҽ�---��y����ת\n");
	printf("a/A��------����/��Զ\n");
	printf("1/2��------Flat/Smooth����ģʽ\n");

	//���ó�ʼ������
    myinit();

	//�趨���ڴ�С�仯�Ļص�����
	glutReshapeFunc(myReshape);

	//�趨���̿��ƵĻص�����
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	
	//��ʼOPENGL��ѭ��
	glutDisplayFunc(display); 

	glutMainLoop();

	return 0;
}

////////////////////////////////////////////////
//�û���ʼ������
void myinit(void)
{
    //your initialization code
	glEnable(GL_DEPTH_TEST);

//	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);

//	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);	//����ķ���������Ƿ���.
	glShadeModel(GL_SMOOTH);
}

//���ڴ�С�仯ʱ�Ļص�����
void myReshape(GLsizei w, GLsizei h)
{
	//�趨����
    glViewport(0, 0, w, h);

	//�趨͸�ӷ�ʽ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
}

void display(void)
{
	//���������Ļ����ɫ���������Ļ����Ȼ���
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(G_shadingMode);

	////////////////////////////////////////////////
	//���ù���
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//���ù�Դ��λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecu);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);

	//���ó�ģ�;���ģʽ
    glMatrixMode(GL_MODELVIEW);

	//���뵥λ������
    glLoadIdentity();

	//����������Z��ƽ��-G_fDistance (ʹ��������λ�������ǰ��)
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////
	//��������

	//��ʵ�巽ʽ��һ�����
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 1.0f, 0.0f);
//	glPolygonOffset(1.0, 1.0);
//	glEnable(GL_POLYGON_OFFSET_FILL);
	glutSolidTeapot(1.0);
//	glDisable(GL_POLYGON_OFFSET_FILL);

	//���߿�ʽ�ٻ�һ��
	glDisable(GL_LIGHTING);
//	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonOffset(-1.0, -1.0);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glutSolidTeapot(1.0);
	glDisable(GL_POLYGON_OFFSET_LINE);
//	glEnable(GL_DEPTH_TEST);

	//����ǰ�󻺳���
	glutSwapBuffers();
}


void processSpecialKeys(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			G_fAngle_horizon -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			G_fAngle_horizon += 5.0f;
			break;
		case GLUT_KEY_UP:
			G_fAngle_vertical -= 5.0f;
			break;
		case GLUT_KEY_DOWN:
			G_fAngle_vertical += 5.0f;
			break;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key,int x,int y)
{
	switch(key) {
		case 97:	//"a"
			G_fDistance -= 0.3f;
			break;
		case 65:		//"A"
			G_fDistance += 0.3f;
			break;
		case 49:	//"1"
			G_shadingMode = GL_FLAT;
			break;
		case 50:	//"2"
			G_shadingMode = GL_SMOOTH;
			break;
		case 27:	//"esc"
			exit(0);
	}
	glutPostRedisplay();
}