// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//需要包含的头文件
#include <stdio.h>
#include <windows.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/freeglut.h>
//#include <GL/glaux.h>
#pragma comment(lib, "glaux.lib")

//定义输出窗口的大小
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

GLint G_textureFilterMode;	//纹理过滤的方式

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

//texture related parameters
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint G_texNameArray[4];

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//响应键盘输入, 从而设定物体移近移远以及旋转的回调函数
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);

//生成棋盘纹理
void makeCheckImage(void);
void loadTexImages(void);
void setTextureFiltering(void);

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
	printf("1键--------GL_NEAREST纹理映射模式\n");
	printf("2键--------GL_LINEAR纹理映射模式\n");
	printf("3键--------GL_NEAREST_MIPMAP_NEAREST纹理映射模式\n");
	printf("4键--------GL_NEAREST_MIPMAP_LINEAR纹理映射模式\n");
	printf("5键--------GL_LINEAR_MIPMAP_NEAREST纹理映射模式\n");
	printf("6键--------GL_LINEAR_MIPMAP_LINEAR纹理映射模式\n");

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

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);

	//setting the lighting parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//设置光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);

	//Setting the textures
	loadTexImages();
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
    glClearColor(0.3f,0.3f,0.3f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////////////////////////////////////
	//初始化模型变换矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//坐标中心向Z轴平移-G_fDistance (使坐标中心位于摄像机前方)
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////
	//绘制物体
	
	//设置物体材质
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecu);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//绘制地板
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
	setTextureFiltering();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 10.0f); glVertex3f(1.0f, -1.0f, -10.0f);
		glTexCoord2f(0.0f, 10.0f); glVertex3f(-1.0f, -1.0f, -10.0f);
	glEnd();
	
	//绘制左墙和右墙
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
	setTextureFiltering();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(7.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -10.0f);
		glTexCoord2f(7.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -10.0f);

		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(7.0f, 0.0f); glVertex3f(1.0f, 1.0f, -10.0f);
		glTexCoord2f(7.0f, 1.0f); glVertex3f(1.0f, -1.0f, -10.0f);
	glEnd();	
	
	//绘制天花板
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[2]);
	setTextureFiltering();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 7.0f); glVertex3f(1.0f, 1.0f, -10.0f);
		glTexCoord2f(0.0f, 7.0f); glVertex3f(-1.0f, 1.0f, -10.0f);
	glEnd();	

	glDisable(GL_TEXTURE_2D);

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
			G_textureFilterMode = GL_NEAREST;
			break;
		case 50:	//"2"
			G_textureFilterMode = GL_LINEAR;
			break;
		case 51:	//"3"
			G_textureFilterMode = GL_NEAREST_MIPMAP_NEAREST;
			break;
		case 52:	//"4"
			G_textureFilterMode = GL_NEAREST_MIPMAP_LINEAR;
			break;
		case 53:	//"5"
			G_textureFilterMode = GL_LINEAR_MIPMAP_NEAREST;
			break;
		case 54:	//"6"
			G_textureFilterMode = GL_LINEAR_MIPMAP_LINEAR;
			break;
		case 27:	//"esc"
			exit(0);
	}
	glutPostRedisplay();
}

void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;	//这是设置纹理的alpha分量
      }
   }
}

void loadTexImages(void)
{
	glGenTextures(3, G_texNameArray);

    //载入地板纹理
	makeCheckImage();
    glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, checkImageWidth, checkImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	
	//载入墙壁纹理
	AUX_RGBImageRec *pBitmap = NULL;
    LPSTR strFileName = "Textures\\brick.bmp";
	pBitmap = auxDIBImageLoad(strFileName);		// 装入位图，并保存数据
	if(pBitmap == NULL)
		exit(0);					// 如果装入位图失败，则退出

    glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	if (pBitmap)	// 释放位图占用的资源
	{	
		if (pBitmap->data)
			free(pBitmap->data);							
		free(pBitmap);					
	}

	//载入天花板纹理
	pBitmap = NULL;
    strFileName = "Textures\\ceiling.bmp";
	pBitmap = auxDIBImageLoad(strFileName);		// 装入位图，并保存数据
	if(pBitmap == NULL)
		exit(0);					// 如果装入位图失败，则退出

    glBindTexture(GL_TEXTURE_2D, G_texNameArray[2]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	if (pBitmap)	// 释放位图占用的资源
	{	
		if (pBitmap->data)
			free(pBitmap->data);							
		free(pBitmap);					
	}
}

void setTextureFiltering(void)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, G_textureFilterMode);
}