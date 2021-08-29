// OpenGLStarter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//��Ҫ������ͷ�ļ�
#include <stdio.h>
#include <windows.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/freeglut.h>
//#include <GL/glaux.h>
#pragma comment(lib, "glaux.lib")

//����������ڵĴ�С
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

GLint G_textureFilterMode;	//������˵ķ�ʽ

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

//texture related parameters
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint G_texNameArray[4];

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//��Ӧ��������, �Ӷ��趨�����ƽ���Զ�Լ���ת�Ļص�����
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);

//������������
void makeCheckImage(void);
void loadTexImages(void);
void setTextureFiltering(void);

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
	printf("1��--------GL_NEAREST����ӳ��ģʽ\n");
	printf("2��--------GL_LINEAR����ӳ��ģʽ\n");
	printf("3��--------GL_NEAREST_MIPMAP_NEAREST����ӳ��ģʽ\n");
	printf("4��--------GL_NEAREST_MIPMAP_LINEAR����ӳ��ģʽ\n");
	printf("5��--------GL_LINEAR_MIPMAP_NEAREST����ӳ��ģʽ\n");
	printf("6��--------GL_LINEAR_MIPMAP_LINEAR����ӳ��ģʽ\n");

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

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);

	//setting the lighting parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//���ù�Դ��λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);

	//Setting the textures
	loadTexImages();
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
    glClearColor(0.3f,0.3f,0.3f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////////////////////////////////////
	//��ʼ��ģ�ͱ任����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//����������Z��ƽ��-G_fDistance (ʹ��������λ�������ǰ��)
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////
	//��������
	
	//�����������
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, G_vMaterialSpecu);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//���Ƶذ�
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
	setTextureFiltering();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 10.0f); glVertex3f(1.0f, -1.0f, -10.0f);
		glTexCoord2f(0.0f, 10.0f); glVertex3f(-1.0f, -1.0f, -10.0f);
	glEnd();
	
	//������ǽ����ǽ
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
	
	//�����컨��
	glBindTexture(GL_TEXTURE_2D, G_texNameArray[2]);
	setTextureFiltering();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 7.0f); glVertex3f(1.0f, 1.0f, -10.0f);
		glTexCoord2f(0.0f, 7.0f); glVertex3f(-1.0f, 1.0f, -10.0f);
	glEnd();	

	glDisable(GL_TEXTURE_2D);

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
         checkImage[i][j][3] = (GLubyte) 255;	//�������������alpha����
      }
   }
}

void loadTexImages(void)
{
	glGenTextures(3, G_texNameArray);

    //����ذ�����
	makeCheckImage();
    glBindTexture(GL_TEXTURE_2D, G_texNameArray[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, checkImageWidth, checkImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	
	//����ǽ������
	AUX_RGBImageRec *pBitmap = NULL;
    LPSTR strFileName = "Textures\\brick.bmp";
	pBitmap = auxDIBImageLoad(strFileName);		// װ��λͼ������������
	if(pBitmap == NULL)
		exit(0);					// ���װ��λͼʧ�ܣ����˳�

    glBindTexture(GL_TEXTURE_2D, G_texNameArray[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	if (pBitmap)	// �ͷ�λͼռ�õ���Դ
	{	
		if (pBitmap->data)
			free(pBitmap->data);							
		free(pBitmap);					
	}

	//�����컨������
	pBitmap = NULL;
    strFileName = "Textures\\ceiling.bmp";
	pBitmap = auxDIBImageLoad(strFileName);		// װ��λͼ������������
	if(pBitmap == NULL)
		exit(0);					// ���װ��λͼʧ�ܣ����˳�

    glBindTexture(GL_TEXTURE_2D, G_texNameArray[2]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	if (pBitmap)	// �ͷ�λͼռ�õ���Դ
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