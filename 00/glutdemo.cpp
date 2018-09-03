#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <GL/glut.h>

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5, 5, -5, 5, 5, 15);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

	return;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0, 0);
	glutWireTeapot(3);
	glFlush();

	return;
}

int main(int argc, char *argv[])
{
	glm::quat q(0,0,0,1);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(300, 300);
	glutCreateWindow("OpenGL 3D View");
	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}