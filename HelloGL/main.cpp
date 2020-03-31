#include "SceneManager.h"
#include <gl/GLU.h>
#include "GL/freeglut.h"

SceneManager* sceneManager;

void InitWindow(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	InitWindow(argc, argv);
	sceneManager = new SceneManager(argc, argv, SCENE_STARFIELD);
	return 0;
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple OpenGL Program");
}