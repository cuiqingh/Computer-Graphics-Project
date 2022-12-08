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

	glDisable(GL_LIGHTING);

	float size = 199.0;
	float height = 100.0;
	//天顶
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, height);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, size, height);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, size, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(size, 1.0, height);
	glEnd();

	//左边
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, size, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, size, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, height);
	glEnd();

	//前边
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, size, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, size, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, size, height);
	glEnd();


	//右边
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(size, size, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(size, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(size, 1.0, height);
	glTexCoord2f(0.0, 1.0); glVertex3f(size, size, height);
	glEnd();

	//后边
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)600 / (GLfloat)600, 1.0f, 400.0f);

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

	glPolygonMode(GL_FRONT, GL_FILL);
	glShadeModel(GL_SMOOTH); 
	glEnable(GL_CULL_FACE);

	GLfloat ambient[] = { 2.0f, 2.0f, 2.0f, 1.0f };		// 整个场景的环境光强度 
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };		//漫射光
	GLfloat position[] = { 100.0f, 180.0f, 180.0f, 1.0f };	//光源位置
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };	//镜面反射参数
	GLfloat mat_shininess[] = { 50.0 };		//高光指数

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
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