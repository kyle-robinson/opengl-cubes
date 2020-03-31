#include "SceneManager.h"
#include <gl/GLU.h>
#include "GL/freeglut.h"

SceneManager* sceneManager;

void InitWindow(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	InitWindow(argc, argv);
	sceneManager = new SceneManager();
	return 0;
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple OpenGL Program");

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 800, 800);
	gluPerspective(90, 1, 1, 1000);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}