#include "SceneMenu.h"
#include <iostream>

SceneMenu::SceneMenu() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	std::cout << "Scene menu loaded." << std::endl;

	glutMainLoop();
}

SceneMenu::~SceneMenu(void)
{
	delete camera;
	camera = NULL;

	delete _lightPosition;
	_lightPosition = NULL;

	delete _lightData;
	_lightData = NULL;

	delete textureStarfield;
	textureStarfield = NULL;

	delete textureCollision;
	textureCollision = NULL;

	delete textureGame;
	textureGame = NULL;

	delete textureLoader;
	textureLoader = NULL;
}

void SceneMenu::InitGL()
{
	GLUTCallbacks::Init(this);

	glutDisplayFunc(GLUTCallbacks::Display);
}

void SceneMenu::InitLighting()
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

void SceneMenu::InitObjects()
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

	textureStarfield = new Texture2D();
	textureCollision = new Texture2D();
	textureGame = new Texture2D();
	textureLoader = new Texture2D();
}

void SceneMenu::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textureStarfield->Load((char*)"Textures/starfield.raw", 512, 512);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.75, 0.25);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.25, 0.25);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.25, 0.75);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.75, 0.75);
	glEnd();

	textureCollision->Load((char*)"Textures/collision.raw", 512, 512);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.25, 0.25);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.75, 0.25);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.75, 0.75);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.25, 0.75);
	glEnd();
	
	textureGame->Load((char*)"Textures/game.raw", 512, 512);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.75, -0.5);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.25, -0.5);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.25, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.75, 0.0);
	glEnd();

	textureLoader->Load((char*)"Textures/object_loader.raw", 512, 512);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.25, -0.5);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.75, -0.5);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.75, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.25, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	Vector3 vMenu = { -0.3f, 1.75f, -1.0f };
	Vector3 vStarfield = { -1.7f, 0.3f, -1.0f };
	Vector3 vCollision = { 0.3f, 0.3f, -1.0f };
	Vector3 vGame = { -1.64f, -1.2f, -1.0f };
	Vector3 vLoadOBJ = { 0.25f, -1.2f, -1.0f };
	Vector3 vExit = { -0.5f, -1.75f, -1.0f };

	Color cRed = { 1.0f, 0.2f, 0.2f };
	Color cYellow = { 1.0f, 1.0f, 0.0f };
	Color cMagenta = { 1.0f, 0.0f, 1.0f };
	Color cGreen = { 0.0f, 1.0f, 0.0f };
	Color cCyan = { 0.0f, 1.0f, 1.0f };
	Color cOrange = { 1.0f, 0.7f, 0.0f };

	DrawString("Main Menu", &vMenu, &cRed);
	DrawString("Press '1' for Starfield Scene", &vStarfield, &cYellow);
	DrawString("Press '2' for Collision Scene", &vCollision, &cMagenta);
	DrawString("Press '3' for Game Scene", &vGame, &cGreen);
	DrawString("Press '4' for OBJ Loader Scene", &vLoadOBJ, &cCyan);
	DrawString("Press 'ESC' to Exit", &vExit, &cOrange);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glFlush();
	glutSwapBuffers();
}

void SceneMenu::Update()
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

void SceneMenu::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glColor3f(color->r, color->g, color->b);
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0, 0.0);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
	glPopMatrix();
}