#define _CRT_SECURE_NO_WARNINGS		//����ĳЩ����
#include <string.h>      
#include <time.h> 
#include "fluid.h"

Fluid* f;

float center[] = { 20, 10, 0 };
float eye[] = { 20, -50, 50 };

void drawScene()
{
	static int count = 0;
	count++;
	if (count > 100) {		//����ˮ��ˢ�¼��
		count = 0;
		f->Evaluate();
	}
	f->draw();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);	//�����ӿ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 150.0f);	//����͸��ͶӰ
}

void init()
{
	srand(unsigned(time(NULL)));
	glEnable(GL_DEPTH_TEST);	//������Ȳ��� 
	f = new Fluid(20, 20, 2, 1, 0.2);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0); 

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawScene();

	glutSwapBuffers();     
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);    
	glutInitWindowSize(600, 600);
	glutCreateWindow("Water simulate1.0");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(glutPostRedisplay);

	init();

	glutMainLoop(); 
	return 0;
}