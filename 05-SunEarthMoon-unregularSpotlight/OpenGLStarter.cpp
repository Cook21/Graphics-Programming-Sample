#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>

const GLfloat Pi = 3.1415926536f;
#define RAD(x) (3.1415926*(x)/180.0)

//摄像机参数
float G_cameraPos[3]={0.0f, 0.0f, -300000.0f};
float G_lookatPos[3]={0.0f, 0.0f, -299990.0f};
float G_angleH = 0.0f;
float G_angleV =0.0f;
//飞船坐标
float flyx=0.0f,flyy=0.0f,flyz=-299990.0f;

//太阳光照参数
float G_vLit0Position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float G_vLit0Ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float G_vMaterialSpecu[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
//聚光光照参数
float G_vLit1Position[4] = { 0.0f, 0.0f,  1738*6.0f, 1.0f };
float spot_direction[]={ 0.0 ,  0.0,1.0 };
//泛光光照参数
float G_vLit2Position[4] = { 0.0f,3395*5.0f,  0.0f, 1.0f };

float selfroat=0.0f;
static int day = 0;

void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);
void idle(int p);//旋转的回调函数
void pressKey(unsigned char key, int x, int y);
void pressSKey(int key, int x, int y);

int main(int argc, char* argv[])//主函数
{ 
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);	//初始化OPENGL显示方式
	glutInitWindowSize (1000, 700); 	//设定OPENGL窗口位置和大小
	glutInitWindowPosition (0, 0);     //打开窗口
	
	glutCreateWindow ("sunearthmoon");
	printf("上、下键---绕x轴旋转\n");
	printf("左、右键---绕y轴旋转\n");
	printf("w/s键------相机拉近/推远\n");
	printf("a/d键------相机右移/左移\n");
	printf("q/e键------相机降低/抬高\n");
	printf("1/2/3键------太阳自发光/聚光灯/泛光灯\n");
	

    myinit();                           	//调用初始化函数
	glutKeyboardFunc(pressKey);
    glutSpecialFunc(pressSKey);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);           	//设定键盘控制的回调函数
	glutTimerFunc(100,idle,0);               //定时器
	//glutIdleFunc( idle );
	glutMainLoop();
	return 0;
}

void myinit(void)  //用户初始化函数
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glShadeModel(GL_SMOOTH);
}
void myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0,1.0f * w / h, 1.0, 4000000);
}
//每桢OpenGL都会调用这个函数，用户应该把显示代码放在这个函数中
void display(void)
{
	//设置清除屏幕的颜色，并清除屏幕和深度缓冲
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	//设置光照
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//设置光源1太阳自发光的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, G_vLit0Ambient);//设置光源2聚光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, G_vLit0Specular);
	glLightf( GL_LIGHT1,GL_SPOT_CUTOFF , 50 );            
    glLightfv( GL_LIGHT1 , GL_SPOT_DIRECTION , spot_direction );
			
	glLightfv(GL_LIGHT2, GL_AMBIENT, G_vLit0Ambient);//设置光源3泛光
	glLightfv(GL_LIGHT2, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, G_vLit0Specular);
	glLightf( GL_LIGHT2 , GL_CONSTANT_ATTENUATION , 2.0);
 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
    gluLookAt(G_cameraPos[0], G_cameraPos[1], G_cameraPos[2], G_lookatPos[0], G_lookatPos[1], G_lookatPos[2], 0, 1, 0); //设置视点，物体世界坐标，相机向上的方向是世界坐标的方向
     // 飞船
	glPushMatrix(); 
	glTranslatef(flyx, flyy,flyz);
    glColor3f(1.0f, 0.0f, 0.0f); 
    glutSolidSphere(0.2, 20, 20); 
	glTranslatef(0.35, 0.0f, 0.0f); 
	glColor3f(0.0f, 1.0f, 0.0f); 
    glutSolidSphere(0.22, 20, 20); 
	glTranslatef(-0.22, 0.0f, 0.0f); 
	glutSolidCube(0.3);

    glPopMatrix();
	// 绘制太阳
	glPushMatrix();
	glRotatef(selfroat, 0.0f, 0.0f, 1.0f); //太阳自转
    glColor3f(0.0f,1.0f, 1.0f); 
    //glutWireSphere(69600, 20, 20);
	glMaterialfv(GL_FRONT, GL_SPECULAR, G_vMaterialSpecu);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
	glutSolidSphere(69600, 20, 20); 
    glPopMatrix();
	// 绘制火星
	glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f); 
	glRotatef(30, 0.0f, -1.0f, 0.0f);//轨道倾斜-30度
	glBegin(GL_LINE_LOOP);//火星轨道
    for(int i=0; i<10000; ++i)
		glVertex2f(228000*cos(2*Pi/1000*i),228000*sin(2*Pi/1000*i));
    glEnd();

	glRotatef(day/365.0*360.0, 0.0f, 0.0f, -1.0f); 
    glTranslatef(228000, 0.0f, 0.0f); 
    //glutWireSphere(3395*3, 20, 20);
	glLightfv(GL_LIGHT2, GL_POSITION, G_vLit2Position);//泛光位置
	glutSolidSphere(3395*3, 20, 20);
	glPopMatrix();

	//glRotatef(30, 0.0f, 1.0f, 0.0f);//轨道倾斜30度
    // 绘制地球
    glColor3f(0.0f, 0.0f, 1.0f); 

    glBegin(GL_LINE_LOOP);//地球轨道
    for(int i=0; i<1000; ++i)
		glVertex2f(150000*cos(2*Pi/1000*i),150000*sin(2*Pi/1000*i));
    glEnd();

	glRotatef(day/365.0*360.0, 0.0f, 0.0f, -1.0f); 
    glTranslatef(150000, 0.0f, 0.0f); 
    //glutWireSphere(6378*3, 20, 20);
	glutSolidSphere(6378*3, 20, 20);

    // 绘制月亮 1
	glColor3f(1.0f, 1.0f, 1.0f); 
	glBegin(GL_LINE_LOOP);//月亮1轨道
    for(int i=0; i<1000; ++i)
		glVertex2f(38000*cos(2*Pi/1000*i),38000*sin(2*Pi/1000*i));
    glEnd();

	glPushMatrix();
    glRotatef(day%30/30.0*360.0 - day/365.0*360.0, 0.0f, 0.0f, -1.0f);
    glTranslatef(38000, 0.0f, 0.0f); 
    //glutWireSphere(1738*6, 20, 20); 
	glutSolidSphere(1738*6, 20, 20);
	glLightfv(GL_LIGHT1, GL_POSITION, G_vLit1Position);//聚光位置
	glPopMatrix();

	 // 绘制月亮 2
	glColor3f(100.0f, 0.0f, 100.0f);
	glRotatef(60, 0.0f, -1.0f, 0.0f);//轨道倾斜-60度
	glBegin(GL_LINE_LOOP);//月亮2轨道
    for(int i=0; i<1000; ++i)
		glVertex2f(80000*cos(2*Pi/1000*i),80000*sin(2*Pi/1000*i));
    glEnd();

	glPushMatrix();
    glRotatef(day%30/30.0*360.0 - day/365.0*360.0, 0.0f, 0.0f, -1.0f);
    glTranslatef(80000, 0.0f, 0.0f); 
    //glutWireSphere(1738*6, 20, 20); 
	glutSolidSphere(1738*6, 20, 20);
	glPopMatrix();
    glFlush(); 
    glutSwapBuffers();
}
void idle(int p)
{
	day++;
	selfroat +=0.1;
	if( day >= 365 )
		day = 0;
	glutTimerFunc(100,idle,0);
	glutPostRedisplay();
}
void pressKey(unsigned char key, int x, int y){//平移
	switch (key) {
	case 'w':
		G_cameraPos[2] += 10000;
		G_lookatPos[2] += 10000;
		          flyz += 10000;break;
	case 's':
		G_cameraPos[2] -= 10000;
		G_lookatPos[2] -= 10000;
		          flyz -= 10000;break;
	case 'a':
		G_cameraPos[0] += 10000;
		G_lookatPos[0] += 10000;
		          flyx += 10000;break;
	case 'd':
		G_cameraPos[0] -= 10000;
		G_lookatPos[0] -= 10000;
		          flyx -= 10000;break;
	case 'q':
		G_cameraPos[1] -= 10000;
		G_lookatPos[1] -= 10000;
		          flyy -= 10000;break;
	case 'e':
		G_cameraPos[1] += 10000;
		G_lookatPos[1] += 10000;
		          flyy += 10000;break;
	case'1'://太阳自发光
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);break;
	case'2'://聚光
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT2);break;
	case'3'://泛光灯
		glEnable(GL_LIGHT2);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);break;


	}
}

void pressSKey(int key, int x, int y){//旋转
//水平旋转的时候还是180°以后有往回摆动了(第一人称有这种情况)
//旋转的的时候会有调频，为什么？？？原因是视点的位置与飞船的位置不一致
//第三人称照相机跟着飞船转动。
	switch (key) {
	case GLUT_KEY_LEFT:
		G_angleH += 0.1;break;
	case GLUT_KEY_RIGHT:
		G_angleH -= 0.1;break;
	case GLUT_KEY_UP:
		G_angleV +=0.1;break;
	case GLUT_KEY_DOWN:
		G_angleV -=0.1;break;				  
	}
		 //G_angleV=( G_angleV>90 ? 90 : G_angleV<-90 ? -90 : G_angleV);
		 //G_lookatPos[0] = G_cameraPos[0] + 10*sin(RAD(G_angleH));
		 //G_lookatPos[2] = G_cameraPos[2] +10*cos(RAD(G_angleH));
		 //G_lookatPos[1] = G_cameraPos[1] + 10*sin(RAD(G_angleV));
		 //G_lookatPos[2] = G_cameraPos[2] +10*cos(RAD(G_angleV));
	G_cameraPos[0]= flyx - 10*sin(RAD(G_angleH));
	G_cameraPos[2]= flyz - 10*cos(RAD(G_angleH));
	G_cameraPos[1]= flyy - 10*sin(RAD(G_angleV));
	G_cameraPos[2]= flyz - 10*cos(RAD(G_angleV));
}