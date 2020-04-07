#include "SceneGame.h"
#include "MeshLoader.h"

#include <iostream>

SceneGame::SceneGame() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	paused = false;
	audioPlaying = false;

	std::cout << "Game scene loaded." << std::endl;

	glutMainLoop();
}

SceneGame::~SceneGame(void)
{
	delete camera;
	camera = NULL;

	delete _lightPosition;
	_lightPosition = NULL;

	delete _lightData;
	_lightData = NULL;

	delete texturePenguins;
	texturePenguins = NULL;

	delete textureStars;
	textureStars = NULL;

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		delete objects[i];
		objects[i] = NULL;
	}
}

void SceneGame::InitGL()
{
	GLUTCallbacks::Init(this);

	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
	glutDisplayFunc(GLUTCallbacks::Display);
}

void SceneGame::InitLighting()
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

void SceneGame::InitObjects()
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

	cubeMesh = MeshLoader::Load((char*)"Objects/cube.txt");

	texturePenguins = new Texture2D();
	texturePenguins->Load((char*)"Textures/penguins.raw", 512, 512);

	textureStars = new Texture2D();
	textureStars->Load((char*)"Textures/stars.raw", 512, 512);

	objects[0] = new Cube(cubeMesh, texturePenguins, 0.0f, 0.0f, -5.0f);
}

void SceneGame::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (paused)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		Vector3 vTitle = { -0.6f, 1.75f, -1.0f };

		Vector3 vPenguin = { -1.7f, 1.25f, -1.0f };
		Vector3 vPenguinW = { -1.7f, 1.05f, -1.0f };
		Vector3 vPenguinA = { -1.7f, 0.85f, -1.0f };
		Vector3 vPenguinS = { -1.7f, 0.65f, -1.0f };
		Vector3 vPenguinD = { -1.7f, 0.45f, -1.0f };

		Vector3 vColour = { 0.25f, 1.25f, -1.0f };
		Vector3 vColourRed = { 0.25f, 1.05f, -1.0f };
		Vector3 vColourGreen = { 0.25f, 0.85f, -1.0f };
		Vector3 vColourBlue = { 0.25f, 0.65f, -1.0f };
		Vector3 vColourCyan = { 0.25f, 0.45f, -1.0f };
		Vector3 vColourMagenta = { 0.25f, 0.25f, -1.0f };
		Vector3 vColourYellow = { 0.25f, 0.05f, -1.0f };
		Vector3 vColourReset = { 0.25f, -0.15f, -1.0f };

		Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

		Color cWhite = { 1.0f, 1.0f, 1.0f };
		Color cRed = { 1.0f, 0.2f, 0.2f };
		Color cGreen = { 0.0f, 1.0f, 0.0f };
		Color cBlue = { 0.2f, 0.2f, 1.0f };
		Color cCyan = { 0.0f, 1.0f, 1.0f };
		Color cMagenta = { 1.0f, 0.0f, 1.0f };
		Color cYellow = { 1.0f, 1.0f, 0.0f };
		Color cOrange = { 1.0f, 0.7f, 0.0f };

		DrawString("Game Scene Controls", &vTitle, &cRed);

		DrawString("Cube Movement", &vPenguin, &cYellow);
		DrawString("'w' - Move cube up", &vPenguinW, &cWhite);
		DrawString("'a' - Move cube left", &vPenguinA, &cWhite);
		DrawString("'s' - Move cube down", &vPenguinS, &cWhite);
		DrawString("'d' - Move cube right", &vPenguinD, &cWhite);

		DrawString("Cube Colour", &vColour, &cOrange);
		DrawString("'r' - Change to red", &vColourRed, &cWhite);
		DrawString("'g' - Change to green", &vColourGreen, &cWhite);
		DrawString("'b' - Change to blue", &vColourBlue, &cWhite);
		DrawString("'c' - Change to cyan", &vColourCyan, &cWhite);
		DrawString("'m' - Change to magenta", &vColourMagenta, &cWhite);
		DrawString("'y' - Change to yellow", &vColourYellow, &cWhite);
		DrawString("'n' - Reset colours", &vColourReset, &cWhite);

		DrawString("'TAB' to return to the scene.", &vReturn, &cRed);

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->Draw();
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

			Vector3 vTitle = { -1.8f, 1.7f, -1.0f };
			Vector3 vReturn = { -0.7f, -1.75f, -1.0f };
			
			Color cWhite = { 1.0f, 1.0f, 1.0f };

			DrawString("OpenGL Game", &vTitle, &cWhite);
			DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}

	glFlush();
	glutSwapBuffers();
}

void SceneGame::Update()
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

	if (!paused)
	{
		if (audioPlaying)
		{
			audioPlaying = false;
			PlaySound(NULL, NULL, 0);
		}

		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->Update();

			if (objects[i]->_position.x > 4.0f)
				objects[i]->_position.x -= 0.1f;
			if (objects[i]->_position.x < -4.0f)
				objects[i]->_position.x += 0.1f;

			if (objects[i]->_position.y > 4.0f)
				objects[i]->_position.y -= 0.1f;
			if (objects[i]->_position.y < -4.0f)
				objects[i]->_position.y += 0.1f;
		}
	}
	else
	{
		if (!audioPlaying)
		{
			audioPlaying = true;
			PlaySound("Audio/mario_elevator.wav", GetModuleHandle(NULL), SND_LOOP | SND_ASYNC);
		}
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void SceneGame::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
		objects[0]->_position.y += 0.1f;
	if (key == 's')
		objects[0]->_position.y -= 0.1f;

	if (key == 'a')
		objects[0]->_position.x -= 0.1f;
	if (key == 'd')
		objects[0]->_position.x += 0.1f;

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
		case 'n':
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

	if (key == 9)
	{
		if (!paused)
			paused = true;
		else
			paused = false;
	}
}

void SceneGame::KeyboardSpecial(int key, int x, int y)
{

}

void SceneGame::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glColor3f(color->r, color->g, color->b);
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);	
	glPopMatrix();
}