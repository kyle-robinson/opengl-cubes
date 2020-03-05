#include "HelloGL.h"
#include "MeshLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

HelloGL::HelloGL(int argc, char* argv[])
{
	// Initialise OpenGL
	InitGL(argc, argv);
	InitLighting(argc, argv);
	
	// Initialise scene objects
	InitObjects();

	// Start main loop
	glutMainLoop();
}

HelloGL::~HelloGL(void)
{
	delete camera;
	camera = NULL;

	delete _lightPosition;
	_lightPosition = NULL;

	delete _lightData;
	_lightData = NULL;

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		delete objects[i];
		objects[i] = NULL;
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

	// Create Objects
	Mesh* cubeMesh = MeshLoader::Load((char*)"cube.txt");
	//Mesh* pyramidMesh = MeshLoader::Load((char*)"pyramid.txt");

	Texture2D* texturePenguins = new Texture2D();
	texturePenguins->Load((char*)"penguins.raw", 512, 512);

	/*Texture2D* textureStars = new Texture2D();
	textureStars->Load((char*)"stars.raw", 512, 512);*/
	
	srand(time(NULL));

	for (int i = 0; i < OBJECTCOUNT; i++) 
	{
		objects[i] = new Cube(cubeMesh, texturePenguins, ((rand() % 400) / 10.0f) - 20.0f, ((rand() % 200) / 10.0f) - 10.0f, - (rand() % 1000) / 10.0f);
	}
	/*for (int i = 500; i < 1000; i++) 
	{
		objects[i] = new Pyramid(pyramidMesh, textureStars, ((rand() % 400) / 10.0f) - 20.0f, ((rand() % 200) / 10.0f) - 10.0f, -(rand() % 1000) / 10.0f);
	}*/
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

	// Set up GLUT function callbacks
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);

	glMatrixMode(GL_PROJECTION);

	// Set the viewport to be the entire window
	glViewport(0, 0, 800, 800);

	// Set the correct perspective
	gluPerspective(90, 1, 1, 1000);

	glMatrixMode(GL_MODELVIEW);

	// Enable Textures
	glEnable(GL_TEXTURE_2D);

	// Enable Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	// Enable Depth Testing
	glEnable(GL_DEPTH_TEST);
	
	// Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void HelloGL::InitLighting(int argc, char* argv[])
{
	_lightPosition = new Vector4();

	_lightPosition->x = 0.0;
	_lightPosition->y = 0.0;
	_lightPosition->z = 1.0;
	_lightPosition->w = 0.0;

	_lightData = new Lighting();

	_lightData->Ambient.x = 0.2;
	_lightData->Ambient.y = 0.2;
	_lightData->Ambient.z = 0.2;
	_lightData->Ambient.w = 1.0;

	_lightData->Diffuse.x = 0.8;
	_lightData->Diffuse.y = 0.8;
	_lightData->Diffuse.z = 0.8;
	_lightData->Diffuse.w = 1.0;

	_lightData->Specular.x = 0.2;
	_lightData->Specular.y = 0.2;
	_lightData->Specular.z = 0.2;
	_lightData->Specular.w = 1.0;
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clears the scene.

	// Draw Objects
	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		objects[i]->Draw();
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

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		objects[i]->Update();
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void HelloGL::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'd')
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->_rotation += 5.0f;
		}
	}

	if (key == 'a')
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->_rotation -= 5.0f;
		}
	}
}