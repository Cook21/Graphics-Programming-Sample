// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//需要包含的头文件
#include <stdio.h>
#include <windows.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/freeglut.h>

//定义输出窗口的大小
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

GLenum  G_shadingMode;	//切换两种明暗模式

//摄像机离物体的距离
float G_fDistance = 3.6f;
//物体的旋转角度 
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

////////////////////////////////////////////////
//光照参数
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

//响应键盘输入, 从而设定物体移近移远以及旋转的回调函数
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);


////////////////////////////////////////////////
//主函数
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	//初始化OPENGL显示方式
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);

	//设定OPENGL窗口位置和大小
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
		
	//打开窗口
	glutCreateWindow ("OpenGL");
	printf("上、下键---绕x轴旋转\n");
	printf("左、右键---绕y轴旋转\n");
	printf("a/A键------拉近/推远\n");
	printf("1/2键------Flat/Smooth绘制模式\n");

	//调用初始化函数
    myinit();

	//设定窗口大小变化的回调函数
	glutReshapeFunc(myReshape);

	//设定键盘控制的回调函数
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	
	//开始OPENGL的循环
	glutDisplayFunc(display); 

	glutMainLoop();

	return 0;
}

////////////////////////////////////////////////
//用户初始化函数
void myinit(void)
{
    //your initialization code
	glEnable(GL_DEPTH_TEST);

//	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);

//	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);	//茶壶的法方向好像是反的.
	glShadeModel(GL_SMOOTH);
}

//窗口大小变化时的回调函数
void myReshape(GLsizei w, GLsizei h)
{
	//设定视区
    glViewport(0, 0, w, h);

	//设定透视方式
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
}

void display(void)
{
	//设置清除屏幕的颜色，并清除屏幕和深度缓冲
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(G_shadingMode);

	////////////////////////////////////////////////
	//设置光照
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//设置光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecu);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);

	//设置成模型矩阵模式
    glMatrixMode(GL_MODELVIEW);

	//载入单位化矩阵
    glLoadIdentity();

	//坐标中心向Z轴平移-G_fDistance (使坐标中心位于摄像机前方)
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////
	//绘制物体

	//以实体方式画一个茶壶
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 1.0f, 0.0f);
//	glPolygonOffset(1.0, 1.0);
//	glEnable(GL_POLYGON_OFFSET_FILL);
	glutSolidTeapot(1.0);
//	glDisable(GL_POLYGON_OFFSET_FILL);

	//以线框方式再画一遍
	glDisable(GL_LIGHTING);
//	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonOffset(-1.0, -1.0);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glutSolidTeapot(1.0);
	glDisable(GL_POLYGON_OFFSET_LINE);
//	glEnable(GL_DEPTH_TEST);

	//交换前后缓冲区
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