#include "SceneOBJ.h"
#include <iostream>

SceneOBJ::SceneOBJ()
{
	InitGL();
	InitLighting();
	InitObjects();

	xPosition = 0.0f;
	yPosition = -1.0f;
	zPosition = -10.0f;

	tankLoaded = true;
	cubeLoaded = false;
	skullLoaded = false;

	std::cout << "OBJ scene loaded." << std::endl;

	glutMainLoop();
}

SceneOBJ::~SceneOBJ(void)
{
	delete camera;
	camera = NULL;

	delete _lightPosition;
	_lightPosition = NULL;

	delete _lightData;
	_lightData = NULL;
}

void SceneOBJ::InitGL()
{
	GLUTCallbacks::Init(this);

	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
	glutDisplayFunc(GLUTCallbacks::Display);
}

void SceneOBJ::InitLighting()
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

void SceneOBJ::InitObjects()
{
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

	tankObj.Load((char*)"Objects/IS7.obj");
	cubeObj.Load((char*)"Objects/cube.obj");
	skullObj.Load((char*)"Objects/skull.obj");
}

void SceneOBJ::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		if (tankLoaded)
		{
			glTranslatef(xPosition, yPosition, zPosition);
			glRotatef(g_rotation, 0, 1, 0);
			glRotatef(90, 0, 1, 0);
			g_rotation++;
			tankObj.Draw();
		}
		else if (cubeLoaded)
		{
			glTranslatef(xPosition, yPosition, zPosition);
			glRotatef(g_rotation, 0, 1, 0);
			glRotatef(90, 0, 1, 0);
			g_rotation++;
			cubeObj.Draw();
		}
		else if (skullLoaded)
		{
			glTranslatef(xPosition, yPosition, zPosition);
			glRotatef(g_rotation, 0, 1, 0);
			glRotatef(90, 0, 1, 0);
			g_rotation++;
			skullObj.Draw();
		}
	glPopMatrix();

	Vector3 v = { -1.8f, 1.7f, -1.0f };
	Color c = { 0.0f, 1.0f, 0.0f };
	DrawString("OBJ Loader", &v, &c);

	glFlush();
	glutSwapBuffers();
}

void SceneOBJ::Update()
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

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void SceneOBJ::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
		yPosition += 0.1f;
	if (key == 'a')
		xPosition -= 0.1f;
	if (key == 's')
		yPosition -= 0.1f;
	if (key == 'd')
		xPosition += 0.1f;

	if (key == 't')
	{
		tankLoaded = true;
		cubeLoaded = false;
		skullLoaded = false;

		xPosition = 0.0f;
		yPosition = -1.0f;
		zPosition = -10.0f;
	}
	if (key == 'c')
	{
		tankLoaded = false;
		cubeLoaded = true;
		skullLoaded = false;

		xPosition = 0.0f;
		yPosition = -0.25f;
		zPosition = -1.0f;
	}
	if (key == 'h')
	{
		tankLoaded = false;
		cubeLoaded = false;
		skullLoaded = true;

		xPosition = 0.0f;
		yPosition = -15.0f;
		zPosition = -30.0f;
	}
}

void SceneOBJ::KeyboardSpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		zPosition -= 0.1f;
	if (key == GLUT_KEY_DOWN)
		zPosition += 0.1f;
}

void SceneOBJ::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
	glPopMatrix();
}