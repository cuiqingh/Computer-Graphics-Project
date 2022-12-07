#define _CRT_SECURE_NO_WARNINGS
#include <string.h>      
#include <time.h> 
#include <math.h>
#include "fluid.h"
#include "texture.h"

GLuint texture[6];
Fluid* f;

static float angleXz = 0.0;
static float angleXy = 0.0;
static float x = 50.0f, y = 100.0f, z = 10.0f;
float PI = 3.14159f;

void skyBox() {
	float size = 199.0;
	float height = 100.0;
	//Ìì¶¥
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, height);
	glTexCoord2f(1.0, 0.0); glVertex3f(size, 1.0, height);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, size, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, size, height);
	glEnd();

	//×ó±ß
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, size, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, size, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, height);
	glEnd();

	//Ç°±ß
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, size, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, size, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, size, height);
	glEnd();


	//ÓÒ±ß
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(size, size, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(size, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, 1.0, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(size, size, height);
	glEnd();

	//ºó±ß
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(size, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(size, 1.0, height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawScene()
{
	static int count = 0;
	count++;
	if (count > 80) {
		count = 0;
		f->Evaluate();
	}
	f->draw();
	skyBox();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 300.0f);
}


void init()
{
	srand(unsigned(time(NULL)));
	glEnable(GL_DEPTH_TEST);	

	glGenTextures(6, texture);
	loadTex(texture);

	f = new Fluid(100, 100, 2, 1, 0.2, texture[0]);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z, float(x + 100 * cos(PI * angleXz / 180.0f)),
			float(y + 100 * sin(PI * angleXy / 180.0f)),
			float(z + 100 * sin(PI * angleXz / 180.0f)), 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	drawScene();

	glutSwapBuffers();
}

void keyBoard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
	case 'A':
		angleXy += 2.0; break;
	case 'd':
	case 'D':
		angleXy -= 2.0; break;
	case 'w':
	case 'W':
		angleXz += 2.0; break;
	case 's':
	case 'S':
		angleXz -= 2.0; break;
	case '1':
		z += (float)sin(PI * angleXz / 180.0f) * 1.0;
		break;
	case '2':
		z -= (float)sin(PI * angleXz / 180.0f) * 1.0;
	}
	
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);    
	glutInitWindowSize(600, 600);
	glutCreateWindow("Water simulate");    

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(glutPostRedisplay);
	glutKeyboardFunc(keyBoard);

	init();

	glutMainLoop(); 
	return 0;
}