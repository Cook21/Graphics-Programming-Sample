// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//需要包含的头文件
#include <stdio.h>
#include <windows.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ImageLoader.h"

#pragma comment(lib, "glew32.lib")


//定义输出窗口的大小
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

GLuint G_vShader_simple;
GLuint G_fShader_simple;
GLuint G_shaderProgram;

//摄像机离物体的距离
float G_fDistance = 3.6f;
//物体的旋转角度 
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

////////////////////////////////////////////////
//光照参数
float G_vLit0Position[4] = { 5.0f, 5.0f, 5.0f, 1.0f };
float G_vLit0Ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
float G_vLit0Diffuse[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
float G_vLit0Specular[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

float G_vMaterialAmbient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vMaterialDiffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vMaterialSpecular[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_iShininess = 50;

//texture related parameters
static GLuint G_texNameArray[2];
GLfloat G_fTextureOffset = 0.0f;

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);
void setShaders(void);
void printShaderInfoLog(GLuint shaderObject);
void printProgramInfoLog(GLuint programObject);
char* textFileRead(const char *textFileName);

//响应键盘输入, 从而设定物体移近移远以及旋转的回调函数
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);
void textureMove(int value);

//生成纹理
void loadTexImages(void);
void setTextureFiltering(void);

////////////////////////////////////////////////
//主函数
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	//初始化OPENGL显示方式
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
//	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);

	//设定OPENGL窗口位置和大小
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
	glutInitWindowPosition (100, 100);
		
	//打开窗口
	glutCreateWindow ("OpenGL");

	//调用初始化函数
    myinit();

	//设定窗口大小变化的回调函数
	glutReshapeFunc(myReshape);

	//设定键盘控制的回调函数
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutTimerFunc(10, textureMove, 0);
	
	//开始OPENGL的循环
	glutDisplayFunc(display); 
//	glutIdleFunc(display);

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
//	glEnable(GL_COLOR_MATERIAL);
//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLenum err = glewInit();   
	if (GLEW_OK != err)   
	{   
		printf("glew initionlize error: %s\n", glewGetErrorString(err));
	}
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else {
		printf("Not totally ready \n");
		exit(1);
	}

	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else 
	{ 
		printf("OpenGL 2.0 not supported\n"); 
		exit(1);
	}
	setShaders();

	//Setting the textures
	loadTexImages();
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
	setTextureFiltering();
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
	setTextureFiltering();
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
//	gluPerspective(60.0, 1.0, 1.0, 30.0);
//  glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 30.0);
}

//每桢OpenGL都会调用这个函数，用户应该把显示代码放在这个函数中
void display(void)
{
	//设置清除屏幕的颜色，并清除屏幕和深度缓冲
    glClearColor(1.0f,1.0f,1.0f,0.0f);
//	glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//设置光照、材质参数
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//设置光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, G_vMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, G_vMaterialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecular);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, G_iShininess);

	//坐标中心向Z轴平移-G_fDistance(使坐标中心位于摄像机前方)，并设置旋转角度
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	//////////////////////////////////////////////
	//设置multi-textures.
	glActiveTexture(GL_TEXTURE0);	//the first texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glActiveTexture(GL_TEXTURE1);	//the second texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
//	glPushMatrix();
	glTranslatef(G_fTextureOffset, 0.0f, 0.0f); //实现文字移动
//	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	////////////////////////////////////////////////
	//绘制一个quad
	glUseProgram(G_shaderProgram);
	GLint texLocation0, texLocation1;
	texLocation0 = glGetUniformLocation(G_shaderProgram, "brickTex");
	glUniform1i(texLocation0, 0);
	texLocation1 = glGetUniformLocation(G_shaderProgram, "textTex");
	glUniform1i(texLocation1, 1);

	glBegin(GL_QUADS);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		
		glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		
		glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		
		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
	glUseProgram(0);

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
		case 27:	//"esc"
			exit(0);
	}
	glutPostRedisplay();
}

void setShaders(void)
{  
    char *vs,*fs;  
  
    G_vShader_simple = glCreateShader(GL_VERTEX_SHADER);  
    G_fShader_simple = glCreateShader(GL_FRAGMENT_SHADER);    
  
    vs = textFileRead("Shader/simple.vert");  
    fs = textFileRead("Shader/simple.frag");  
  
    const char *vv = vs;  
    const char *ff = fs;  
  
    glShaderSource(G_vShader_simple, 1, &vv, NULL);  
    glShaderSource(G_fShader_simple, 1, &ff, NULL);  
  
	free(vs);
	free(fs);
	/////////////////////////////////////////////////////////
    glCompileShader(G_vShader_simple);  
    glCompileShader(G_fShader_simple);  

	int checkResult;
	glGetShaderiv(G_vShader_simple, GL_COMPILE_STATUS, &checkResult);  
	if(GL_FALSE == checkResult)
	{
		printf("vertex shader compile error\n");
		printShaderInfoLog(G_vShader_simple);
	}

	glGetShaderiv(G_fShader_simple, GL_COMPILE_STATUS, &checkResult);  
	if(GL_FALSE == checkResult)
	{
		printf("fragment shader compile error\n");
		printShaderInfoLog(G_fShader_simple);
	}
	////////////////////////////////////////////////////////////
    G_shaderProgram = glCreateProgram();  
  
    glAttachShader(G_shaderProgram, G_vShader_simple);  
    glAttachShader(G_shaderProgram, G_fShader_simple);  
  
    glLinkProgram(G_shaderProgram);  
	glGetProgramiv(G_fShader_simple, GL_LINK_STATUS, &checkResult);  
	if(GL_FALSE == checkResult)
	{
		printf("shader link error\n");
		printProgramInfoLog(G_shaderProgram);
	}
//    glUseProgram(G_shaderProgram);  
}  

char* textFileRead(const char *textFileName)
{
	FILE *fp;

    if(NULL == (fp = fopen(textFileName, "r")))  
    {  
        printf("text file read error\n");  
		exit(1);  
    }  
  
    char ch;
	int fileLen = 0;
	//首先得到文件长度
	while(EOF != (ch=fgetc(fp)))  
    {  
        fileLen ++;  
    }

	char *fileStr = (char *)malloc((fileLen+1)*sizeof(char));
	//第二次读取文件
	rewind(fp);
	int i = 0;
    while(EOF != (ch=fgetc(fp)))  
    {  
        fileStr[i] = ch;
		i++;
    }  
	fileStr[fileLen] = '\0';	//注意这个一定要加。
  
    fclose(fp);
	return fileStr;
}

void printShaderInfoLog(GLuint shaderObject)
{
	GLint logLen = 0;
	GLint writtenLen = 0;
	GLchar* info_log;

	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH , &logLen);       

	if (logLen > 1)
	{
		info_log = (GLchar*)malloc(logLen);
//		glGetInfoLogARB(shaderObject, logLen, &writtenLen, info_log);	//也许这是老版本的函数了。
		glGetShaderInfoLog(shaderObject, logLen, &writtenLen, info_log);  
//		printf("Information log: \n");
		printf("%s\n", info_log);
		free (info_log);
	}
}

void printProgramInfoLog(GLuint programObject)
{
	GLint logLen = 0;
	GLint writtenLen = 0;
	GLchar* info_log;

	glGetShaderiv(programObject, GL_INFO_LOG_LENGTH , &logLen);       

	if (logLen > 1)
	{
		info_log = (GLchar*)malloc(logLen);
//		glGetInfoLogARB(shaderObject, logLen, &writtenLen, info_log);
		glGetProgramInfoLog(programObject, logLen, &writtenLen, info_log);  
//		printf("Information log: \n");
		printf("%s\n", info_log);
		free (info_log);
	}
}

void loadTexImages(void)
{
	glGenTextures(2, G_texNameArray);

    //载入brick纹理
	GLbyte *pBits = NULL;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	const char *szFileName = "Textures\\brick.bmp";
	
	// Read the texture bits
	pBits = gltReadBMPBits(szFileName, &nWidth, &nHeight);
	if(pBits == NULL) 
		return;

	glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nWidth, nHeight, GL_BGR, GL_UNSIGNED_BYTE, pBits);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    free(pBits);

    //载入text.bmp纹理
	pBits = NULL;
	szFileName = "Textures\\text.bmp";
	
	// Read the texture bits
	pBits = gltReadBMPBits(szFileName, &nWidth, &nHeight);
	if(pBits == NULL) 
		return;

	glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nWidth, nHeight, GL_BGR, GL_UNSIGNED_BYTE, pBits);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    free(pBits);
}

void setTextureFiltering(void)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void textureMove(int value)
{
	G_fTextureOffset -= 0.005f;
	glutTimerFunc(10, textureMove, 0);
	glutPostRedisplay();
}