#include <stdio.h>
#include <Windows.h>
#include <GL/freeglut.h>

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//定义窗口大小
	glutInitWindowSize(300, 300);
	//定义窗口位置
	glutInitWindowPosition(100, 100);
	//创建窗口
	glutCreateWindow("testgl");
	const char* version = (const char*)glGetString(GL_VERSION);
	printf("OpenGL 版本：%s\n", version);
	glutMainLoop();  //进入GLUT事件处理循环，无限循环
	return 0;
}
