#include "SceneMenu.h"

SceneMenu::SceneMenu() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();
	glutMainLoop();
}

SceneMenu::~SceneMenu(void)
{
	Scene::~Scene();
	
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
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.75, 0.15);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.25, 0.15);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.25, 0.65);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.75, 0.65);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureCollision->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.25, 0.15);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.75, 0.15);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.75, 0.65);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.25, 0.65);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureGame->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.75, -0.6);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.25, -0.6);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.25, -0.1);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.75, -0.1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureLoader->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.25, -0.6);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.75, -0.6);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.75, -0.1);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.25, -0.1);
	glEnd();
}

void SceneMenu::DrawUI()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	Vector3 vMenu =			{ -0.375f, 1.75f, -1.0f };
	
	Vector3 vStarfield =	{ -1.58f, 1.35f, -1.0f };
	Vector3 vStarfieldKey =	{ -1.3f, 0.2f, -1.0f };

	Vector3 vCollision =	{ 0.38f, 1.35f, -1.0f };
	Vector3 vCollisionKey =	{ 0.725f, 0.2f, -1.0f };
	
	Vector3 vGame =			{ -1.425f, -0.15f, -1.0f };
	Vector3 vGameKey =		{ -1.3f, -1.3f, -1.0f };
	
	Vector3 vLoadOBJ =		{ 0.6f, -0.15f, -1.0f };
	Vector3 vLoadOBJKey =	{ 0.725f, -1.3f, -1.0f };
	
	Vector3 vExit =			{ -0.5f, -1.75f, -1.0f };

	Color cWhite =			{ 1.0f, 1.0f, 1.0f };
	
	Color cBrightGreen =	{ 0.1f, 0.8f, 0.4f };
	Color cPaleGreen =		{ 0.8f, 1.0f, 0.8f };
	
	Color cSea =			{ 0.2f, 0.5f, 1.0f };
	Color cLightBlue =		{ 0.4f, 1.0f, 1.0f };
	
	Color cDarkPink =		{ 0.9f, 0.1f, 0.8f };
	Color cPalePink =		{ 1.0f, 0.8f, 1.0f };
	
	Color cPurple =			{ 0.6f, 0.2f, 0.9f };
	Color cLilac =			{ 0.9f, 0.8f, 1.0f };

	DrawString("M A I N    M E N U", &vMenu, &cWhite);
												 				  
	DrawString("S T A R F I E L D    S C E N E", &vStarfield, &cBrightGreen);
	DrawString("p r e s s    `1'", &vStarfieldKey, &cPaleGreen);
												 				  
	DrawString("C O L L I S I O N    S C E N E", &vCollision, &cSea);
	DrawString("p r e s s    `2'", &vCollisionKey, &cLightBlue);
												 
	DrawString("G A M E    S C E N E", &vGame, &cDarkPink);
	DrawString("p r e s s    `3'", &vGameKey, &cPalePink);
												 
	DrawString("O B J    L O A D E R", &vLoadOBJ, &cPurple);
	DrawString("p r e s s    `4'", &vLoadOBJKey, &cLilac);
	
	DrawString("`E S C'    t o    e x i t    . . .", &vExit, &cWhite);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}