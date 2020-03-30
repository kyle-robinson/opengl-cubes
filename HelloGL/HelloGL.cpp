#include "HelloGL.h"
#include "MeshLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitLighting(argc, argv);
	InitObjects();

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

void HelloGL::InitGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Simple OpenGL Program");

	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);

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

	// Set up objects
	cubeMesh = MeshLoader::Load((char*)"Objects/cube.txt");

	texturePenguins = new Texture2D();
	texturePenguins->Load((char*)"Textures/penguins.raw", 512, 512);

	textureStars = new Texture2D();
	textureStars->Load((char*)"Textures/stars.raw", 512, 512);
	
	srand(time(NULL));

	for (int i = 0; i < OBJECTCOUNT; i++) 
	{
		objects[i] = new Cube(cubeMesh, texturePenguins, ((rand() % 400) / 10.0f) - 20.0f, ((rand() % 200) / 10.0f) - 10.0f, - (rand() % 1000) / 10.0f);
	}
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		objects[i]->Draw();
	}

	Vector3 v = { -1.8f, 1.7f, -1.0f };
	Color c = { 0.0f, 1.0f, 0.0f };
	DrawString("Penguin Cubes", &v, &c);

	glFlush();
	glutSwapBuffers();
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
		if (objects[i]->_position.z > 10.0f)
		{
			objects[i]->_position.z = -100.0f;
		}
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void HelloGL::Keyboard(unsigned char key, int x, int y)
{	
	// Object Movement
	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		if (key == 'd')
			objects[i]->_rotation += 5.0f;
		else if (key == 'a')
			objects[i]->_rotation -= 10.0f;	
	}

	// Change Texture
	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		if (key == 's')
			objects[i] = new Cube(cubeMesh, textureStars, objects[i]->_position.x, objects[i]->_position.y, objects[i]->_position.z);
		else if (key == 'p')
			objects[i] = new Cube(cubeMesh, texturePenguins, objects[i]->_position.x, objects[i]->_position.y, objects[i]->_position.z);
	}

	// Change Colour - RBG W CMY
	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		switch (key)
		{
		case 'r':
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 0.0f;
			break;
		case 'g':
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			break;
		case 'b':
			objects[i]->red = 0.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'w':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'c':
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'm':
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'y':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			break;
		}
	}
}

void HelloGL::KeyboardSpecial(int key, int x, int y)
{
	// Camera Movement
	if (key == GLUT_KEY_UP)
		camera->eye.y -= MOVEMENT_SPEED;
	else if (key == GLUT_KEY_DOWN)
		camera->eye.y += MOVEMENT_SPEED;
	else if (key == GLUT_KEY_LEFT)
		camera->eye.x += MOVEMENT_SPEED;
	else if (key == GLUT_KEY_RIGHT)
		camera->eye.x -= MOVEMENT_SPEED;
}

void HelloGL::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
	glPopMatrix();
}