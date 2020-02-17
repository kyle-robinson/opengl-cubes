#include "HelloGL.h"

HelloGL::HelloGL(int argc, char* argv[])
{
	// Initialise scene objects
	InitObjects();

	// Initialise OpenGL
	InitGL(argc, argv);	

	// Start main loop
	glutMainLoop();
}

HelloGL::~HelloGL(void)
{
	delete camera;
	camera = NULL;

	for (int i = 0; i < CUBECOUNT; i++)
	{
		delete cube[i];
		cube[i] = NULL;
	}
}

void HelloGL::InitObjects()
{
	// Set up camera
	camera = new Camera();

	camera->eye.x = 0.0f;
	camera->eye.y = 0.0f;
	camera->eye.z = 1.0f;

	camera->center.x = 0.0f;
	camera->center.y = 0.0f;
	camera->center.z = 0.0f;

	camera->up.x = 0.0f;
	camera->up.y = 1.0f;
	camera->up.z = 0.0f;

	// Create cube objects
	Cube::Load((char*)"cube.txt");
	for (int i = 0; i < CUBECOUNT; i++)
	{
		cube[i] = new Cube(((rand() % 400) / 10.0f) - 20.0f, ((rand() % 200) / 10.0f) - 10.0f, -(rand() % 1000) / 10.0f);
	}
}

void HelloGL::InitGL(int argc, char* argv[])
{
	// Set up GLUT callbacks
	GLUTCallbacks::Init(this);

	// Create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple OpenGL Program");

	// Enable Depth Testing
	glEnable(GL_DEPTH_TEST);

	// Set up keyboard input
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	glMatrixMode(GL_PROJECTION);

	// Set the viewport to be the entire window
	glViewport(0, 0, 800, 800);

	// Set the correct perspective
	gluPerspective(90, 1, 1, 1000);

	glMatrixMode(GL_MODELVIEW);

	// Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clears the scene.

	// Draw Methods
	for (int i = 0; i < CUBECOUNT; i++)
	{
		cube[i]->Draw();
	}

	glFlush(); // Flushes the scene drawn to the graphics card.
	glutSwapBuffers(); // Swaps the buffers
}

void HelloGL::Update()
{
	glLoadIdentity();

	gluLookAt(
		camera->eye.x,
		camera->eye.y,
		camera->eye.z,
		
		camera->center.x,
		camera->center.y,
		camera->center.z,
		
		camera->up.x,
		camera->up.y,
		camera->up.z
	);

	for (int i = 0; i < CUBECOUNT; i++)
	{
		cube[i]->Update();
	}

	glutPostRedisplay();
}

void HelloGL::Keyboard(unsigned char key, int x, int y)
{
	/*if (key == 'd')
	{
		rotation += 5.0f;
		rotationRect += 5.0f;
		rotationSquare += 5.0f;
		rotationTriangle += 5.0f;
	}

	if (key == 'a')
	{
		rotation -= 5.0f;
		rotationRect -= 5.0f;
		rotationSquare -= 5.0f;
		rotationTriangle -= 5.0f;
	}*/
}