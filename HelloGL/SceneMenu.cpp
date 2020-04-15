#include "SceneMenu.h"
#include <iostream>

SceneMenu::SceneMenu() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();
	glutMainLoop();
}

SceneMenu::~SceneMenu(void)
{
	delete textureStarfield;
	textureStarfield = NULL;

	delete textureCollision;
	textureCollision = NULL;

	delete textureGame;
	textureGame = NULL;

	delete textureLoader;
	textureLoader = NULL;

	delete _material;
	_material = NULL;
}

void SceneMenu::InitGL()
{
	Scene::InitGL();
	GLUTCallbacks::Init(this);
	glutDisplayFunc(GLUTCallbacks::Display);
}

void SceneMenu::InitLighting()
{
	Scene::InitLighting();

	_material = new Material();

	_material->Ambient.x = 1.0;
	_material->Ambient.y = 1.0;
	_material->Ambient.z = 1.0;
	_material->Ambient.w = 1.0;

	_material->Diffuse.x = 1.0;
	_material->Diffuse.y = 1.0;
	_material->Diffuse.z = 1.0;
	_material->Diffuse.w = 1.0;

	_material->Specular.x = 1.0;
	_material->Specular.y = 1.0;
	_material->Specular.z = 1.0;
	_material->Specular.w = 1.0;

	_material->Shininess = 100.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->Ambient.x));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->Diffuse.x));
	glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->Specular.x));
	glMaterialf(GL_FRONT, GL_SHININESS, _material->Shininess);
}

void SceneMenu::InitObjects()
{
	Scene::InitObjects();

	textureStarfield = new Texture2D();
	textureStarfield->Load((char*)"Textures/starfield.raw", 512, 512);
	
	textureCollision = new Texture2D();
	textureCollision->Load((char*)"Textures/collision.raw", 512, 512);
	
	textureGame = new Texture2D();
	textureGame->Load((char*)"Textures/game.raw", 512, 512);
	
	textureLoader = new Texture2D();
	textureLoader->Load((char*)"Textures/object_loader.raw", 512, 512);
}

void SceneMenu::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawQuads();
	DrawUI();

	glFlush();
	glutSwapBuffers();
}

void SceneMenu::Update()
{
	glLoadIdentity();

	Scene::Update();

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
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
	glPopMatrix();
}

void SceneMenu::DrawQuads()
{
	glBindTexture(GL_TEXTURE_2D, textureStarfield->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.75, 0.25);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.25, 0.25);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.25, 0.75);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.75, 0.75);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureCollision->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.25, 0.25);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.75, 0.25);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.75, 0.75);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.25, 0.75);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureGame->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.75, -0.5);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.25, -0.5);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.25, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.75, 0.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureLoader->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.25, -0.5);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.75, -0.5);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.75, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.25, 0.0);
	glEnd();
}

void SceneMenu::DrawUI()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	Vector3 vMenu = { -0.2f, 1.75f, -1.0f };
	Vector3 vStarfield = { -1.58f, 0.3f, -1.0f };
	Vector3 vCollision = { 0.42f, 0.3f, -1.0f };
	Vector3 vGame = { -1.52f, -1.2f, -1.0f };
	Vector3 vLoadOBJ = { 0.35f, -1.2f, -1.0f };
	Vector3 vExit = { -0.4f, -1.75f, -1.0f };

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
}