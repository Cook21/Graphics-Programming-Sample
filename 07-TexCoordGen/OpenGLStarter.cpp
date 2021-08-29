// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//需要包含的头文件
#include <stdio.h>
#include <windows.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/freeglut.h>
#include "ImageLoader.h"

//定义输出窗口的大小
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

//摄像机离物体的距离
float G_fDistance = 3.6f;
//物体的旋转角度 
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

//1D texture
#define	G_stripeImageWidth 32
GLubyte G_stripeImage[4*G_stripeImageWidth];

////////////////////////////////////////////////
//光照参数
float G_vLit0Position[4] = { 5.0f, 0.0f, 5.0f, 1.0f };
float G_vLit0Ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float G_vMaterialSpecu[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit1Position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

//texture related parameters
static GLuint G_texNameArray[3];
GLint G_textureNo = 0;
GLint G_texGenMode = GL_OBJECT_LINEAR;
GLenum G_texGenPlane = GL_OBJECT_PLANE;
GLfloat G_texGenCoeff_s[4] = {1.0, 0.0, 0.0, 10.0};
GLfloat G_texGenCoeff_t[4] = {0.0, 1.0, 0.0, 10.0};

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//响应键盘输入, 从而设定物体移近移远以及旋转的回调函数
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);

//生成纹理
void loadTexImages(void);
void makeStripeImage(void);
void setTexCoorGenPara(void);

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
	printf("1, 2, 3键--切换纹理\n");
	printf("e键--------GL_EYE_PLANE/GL_OBJECT_PLANE\n");


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

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glClearColor(1.0f,1.0f,1.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//初始化模型变换矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	////////////////////////////////////////////////
	//设置物体材质
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecu);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);		//注意，如果把这里的alpha设为0.0，那么GL_MODULATE将会出现画不出物体的问题
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//Bind textures
	if(G_textureNo == 0)
	{
		glEnable(GL_TEXTURE_1D);
		glBindTexture(GL_TEXTURE_1D, G_texNameArray[0]);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, G_texNameArray[G_textureNo]);
	}
	setTexCoorGenPara();

	//设置完纹理坐标自动生成，再设置物体的模型变换
	//坐标中心向Z轴平移-G_fDistance (使坐标中心位于摄像机前方)
	glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);
//	glRotatef(45, 0.0f, 0.0f, 1.0f);
	//rendering objects
	glPushMatrix();
	glTranslatef(-1.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);		//也许是茶壶本身的问题，需转90度方效果正常
	glutSolidTeapot(1.0);
	glPopMatrix();

	glBegin(GL_TRIANGLES);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(3.0f, -1.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);
	glEnd();	
	
	glDisable(GL_TEXTURE_1D);
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
		case 49:		//"1"
			G_textureNo = 0;
			break;
		case 50:		//"2"
			G_textureNo = 1;
			break;
		case 51:		//"3"
			G_textureNo = 2;
			break;
		case 101:		//"e"
			if(G_texGenMode == GL_OBJECT_LINEAR)
			{
				G_texGenMode = GL_EYE_LINEAR;
				G_texGenPlane = GL_EYE_PLANE;
			}
			else
			{
				G_texGenMode = GL_OBJECT_LINEAR;
				G_texGenPlane = GL_OBJECT_PLANE;
			}
			glutPostRedisplay();
			break;
		case 27:	//"esc"
			exit(0);
	}
	glutPostRedisplay();
}

void loadTexImages(void)
{
	glGenTextures(3, G_texNameArray);

	//一维纹理
	makeStripeImage();
	glBindTexture(GL_TEXTURE_1D, G_texNameArray[0]);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, G_stripeImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, G_stripeImage);	


	///////////////////////////////////////////////////////////
    //载入brick.bmp纹理
	GLbyte *pBits = NULL;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	const char *szFileName = "Textures\\brick.bmp";
	
	// Read the texture bits
	pBits = gltReadBMPBits(szFileName, &nWidth, &nHeight);
	if(pBits == NULL) 
		return;

	glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nWidth, nHeight, GL_RGB, GL_UNSIGNED_BYTE, pBits);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    free(pBits);

	///////////////////////////////////////////////////////////
    //载入tree.tga纹理
	pBits = NULL;
	szFileName = "Textures\\tree.tga";
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return;

	glBindTexture(GL_TEXTURE_2D, G_texNameArray[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, nComponents, nWidth, nHeight, eFormat, GL_UNSIGNED_BYTE, pBits);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    free(pBits);
}

void makeStripeImage(void)
{
   int j;
    
   for (j = 0; j < G_stripeImageWidth; j++) {
      G_stripeImage[4*j] = (GLubyte) ((j<=4) ? 255 : 0);
      G_stripeImage[4*j+1] = (GLubyte) ((j>4) ? 255 : 0);
      G_stripeImage[4*j+2] = (GLubyte) 0;
      G_stripeImage[4*j+3] = (GLubyte) 128;
   }
}

void setTexCoorGenPara(void)
{
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, G_texGenMode);
	glTexGenfv(GL_S, G_texGenPlane, G_texGenCoeff_s);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, G_texGenMode);
	glTexGenfv(GL_T, G_texGenPlane, G_texGenCoeff_t);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}