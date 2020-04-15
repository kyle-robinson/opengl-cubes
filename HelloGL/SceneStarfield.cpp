#include "SceneStarfield.h"
#include "MeshLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneStarfield::SceneStarfield() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	zMoving = cRotating = true;

	glutMainLoop();
}

SceneStarfield::~SceneStarfield(void)
{
	delete cubeMesh;
	cubeMesh = NULL;
	
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

void SceneStarfield::InitGL()
{
	Scene::InitGL();
	GLUTCallbacks::Init(this);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
}

void SceneStarfield::InitLighting()
{
	Scene::InitLighting();
}

void SceneStarfield::InitObjects()
{
	Scene::InitObjects();

	cubeMesh = MeshLoader::Load((char*)"Objects/cube.txt");

	texturePenguins = new Texture2D();
	texturePenguins->Load((char*)"Textures/penguins.raw", 512, 512);

	textureStars = new Texture2D();
	textureStars->Load((char*)"Textures/stars.raw", 512, 512);

	for (int i = 0; i < OBJECTCOUNT; i++)
		objects[i] = new Cube(cubeMesh, texturePenguins, ((rand() % 400) / 10.0f) - 20.0f, ((rand() % 200) / 10.0f) - 10.0f, - (rand() % 1000) / 10.0f);
}

void SceneStarfield::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawMenu();

	if (!paused)
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
			objects[i]->Draw();

		if (camera->eye.x == 0.0f)
			DrawUI();
	}

	glFlush();
	glutSwapBuffers();
}

void SceneStarfield::Update()
{
	glLoadIdentity();
	
	Scene::Update();
	SceneAudio();
	
	CameraMovement();

	if (!paused)
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->Update();
			CubeMovement(i);
		}
	}
	else
	{
		ResetCamera();
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void SceneStarfield::Keyboard(unsigned char key, int x, int y)
{	
	if (key == 'i')
		ResetCamera();
	
	if (key == 'e')
	{
		if (!cRotating)
			cRotating = true;
		else
			cRotating = false;
	}

	if (key == 'z')
	{
		if (!zReverse)
			zReverse = true;
		else
			zReverse = false;
	}

	if (key == 'q')
	{
		if (!zMoving)
			zMoving = true;
		else
			zMoving = false;
	}
	
	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		if (key == 'w')
		{
			objects[i]->axisX = 1.0f;
			objects[i]->axisY = 0.0f;
			objects[i]->axisZ = 0.0f;

			objects[i]->_rotation -= CUBE_ROTATION;
			cRotating = false;
		}

		if (key == 's')
		{
			objects[i]->axisX = 1.0f;
			objects[i]->axisY = 0.0f;
			objects[i]->axisZ = 0.0f;

			objects[i]->_rotation += CUBE_ROTATION;
			cRotating = false;
		}
		
		if (key == 'a')
		{
			objects[i]->axisX = 0.0f;
			objects[i]->axisY = 1.0f;
			objects[i]->axisZ = 0.0f;

			objects[i]->_rotation -= CUBE_ROTATION;
			cRotating = false;
		}

		if (key == 'd')
		{
			objects[i]->axisX = 0.0f;
			objects[i]->axisY = 1.0f;
			objects[i]->axisZ = 0.0f;

			objects[i]->_rotation += CUBE_ROTATION;
			cRotating = false;
		}

		if (key == 'l')
		{
			textureAudio = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			objects[i] = new Cube(cubeMesh, textureStars, objects[i]->_position.x, objects[i]->_position.y, objects[i]->_position.z);
		}
		
		if (key == 'p')
		{
			textureAudio = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			objects[i] = new Cube(cubeMesh, texturePenguins, objects[i]->_position.x, objects[i]->_position.y, objects[i]->_position.z);
		}

		if (key == 'r')
		{
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;

			colorAudio = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
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

void SceneStarfield::KeyboardSpecial(int key, int x, int y)
{
	if (!paused)
	{
		if (key == GLUT_KEY_UP)
		{
			camera->eye.z -= MOVEMENT_SPEED;
			camera->center.z -= MOVEMENT_SPEED;
			camera->up.z -= MOVEMENT_SPEED;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			camera->eye.z += MOVEMENT_SPEED;
			camera->center.z += MOVEMENT_SPEED;
			camera->up.z += MOVEMENT_SPEED;
		}
		else if (key == GLUT_KEY_LEFT && camera->eye.z == 1.0f)
			camera->eye.x += MOVEMENT_SPEED / 10;
		else if (key == GLUT_KEY_RIGHT && camera->eye.z == 1.0f)
			camera->eye.x -= MOVEMENT_SPEED / 10;

		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			switch (key)
			{
			case GLUT_KEY_F1:
				objects[i]->red = 1.0f;
				objects[i]->green = 0.0f;
				objects[i]->blue = 0.0f;

				colorAudio = colorIsRed = true;
				colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
				break;

			case GLUT_KEY_F2:
				objects[i]->red = 0.0f;
				objects[i]->green = 1.0f;
				objects[i]->blue = 0.0f;

				colorAudio = colorIsGreen = true;
				colorIsRed = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
				break;

			case GLUT_KEY_F3:
				objects[i]->red = 0.0f;
				objects[i]->green = 0.0f;
				objects[i]->blue = 1.0f;

				colorAudio = colorIsBlue = true;
				colorIsRed = colorIsGreen = colorIsCyan = colorIsMagenta = colorIsYellow = false;
				break;

			case GLUT_KEY_F4:
				objects[i]->red = 0.0f;
				objects[i]->green = 1.0f;
				objects[i]->blue = 1.0f;

				colorAudio = colorIsCyan = true;
				colorIsRed = colorIsGreen = colorIsBlue = colorIsMagenta = colorIsYellow = false;
				break;

			case GLUT_KEY_F5:
				objects[i]->red = 1.0f;
				objects[i]->green = 0.0f;
				objects[i]->blue = 1.0f;

				colorAudio = colorIsMagenta = true;
				colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsYellow = false;
				break;

			case GLUT_KEY_F6:
				objects[i]->red = 1.0f;
				objects[i]->green = 1.0f;
				objects[i]->blue = 0.0f;

				colorAudio = colorIsYellow = true;
				colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = false;
				break;
			}
		}
	}
}

void SceneStarfield::KeyboardUp(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'd')
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->axisX = 1.0f;
			objects[i]->axisY = 0.0f;
			objects[i]->axisZ = 0.0f;
		}
	}

	if (key == 'w' || key == 's' || key == 'a' || key == 'd')
		cRotating = true;
}

void SceneStarfield::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glColor3f(color->r, color->g, color->b);
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
	glPopMatrix();
}

void SceneStarfield::DrawUI()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Vector3 vTitle =	{ -1.8f, 1.7f, camera->eye.z - 2.0f };
	Vector3 vReturn =	{ -1.05f, -1.75f, camera->eye.z - 2.0f };
	Vector3 vPosition = { 0.5f, 1.7f, camera->eye.z - 2.0f };

	Color cWhite =		 { 1.0f, 1.0f, 1.0f };
	Color cBrightGreen = { 0.1f, 0.8f, 0.4f };
	Color cLightGreen =	 { 0.5f, 1.0f, 0.6f };
	Color cPaleGreen =	 { 0.8f, 1.0f, 0.8f };

	DrawString("S T A R F I E L D    S C E N E", &vTitle, &cBrightGreen);
	DrawString("`T A B'    t o    v i e w    s c e n e    c o n t r o l s    . . .", &vReturn, &cPaleGreen);

	glEnable(GL_LIGHTING);

	if (colorIsRed)
		DrawString("Colour Changed to Red", &vPosition, &cWhite);
	else if (colorIsGreen)
		DrawString("Colour Changed to Green", &vPosition, &cWhite);
	else if (colorIsBlue)
		DrawString("Colour Changed to Blue", &vPosition, &cWhite);
	else if (colorIsCyan)
		DrawString("Colour Changed to Cyan", &vPosition, &cWhite);
	else if (colorIsMagenta)
		DrawString("Colour Changed to Magenta", &vPosition, &cWhite);
	else if (colorIsYellow)
		DrawString("Colour Changed to Yellow", &vPosition, &cWhite);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}

void SceneStarfield::DrawMenu()
{
	if (paused)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		Vector3 vTitle =			{ -1.0f, 1.75f, -1.0f };

		Vector3 vCamera =			{ -1.5f, 1.4f, -1.0f };
		Vector3 vCameraReset =		{ -1.5f, 1.2f, -1.0f };
		Vector3 vCameraUp =			{ -1.5f, 1.0f, -1.0f };
		Vector3 vCameraDown =		{ -1.5f, 0.8f, -1.0f };
		Vector3 vCameraLeft =		{ -1.5f, 0.6f, -1.0f };
		Vector3 vCameraRight =		{ -1.5f, 0.4f, -1.0f };

		Vector3 vMove =				{ -1.5f, 0.1f, -1.0f };
		Vector3 vMoveBackward =		{ -1.5f, -0.1f, -1.0f };
		Vector3 vMoveLeft =			{ -1.5f, -0.3f, -1.0f };
		Vector3 vMoveForward =		{ -1.5f, -0.5f, -1.0f };
		Vector3 vMoveRight =		{ -1.5f, -0.7f, -1.0f };
		Vector3 vMoveRotation =		{ -1.5f, -0.9f, -1.0f };
		Vector3 vMoveDirection =	{ -1.5f, -1.1f, -1.0f };
		Vector3 vMoveMovement =		{ -1.5f, -1.3f, -1.0f };

		Vector3 vColour =			{ 0.45f, 1.4f, -1.0f };
		Vector3 vColourRed =		{ 0.45f, 1.2f, -1.0f };
		Vector3 vColourGreen =		{ 0.45f, 1.0f, -1.0f };
		Vector3 vColourBlue =		{ 0.45f, 0.8f, -1.0f };
		Vector3 vColourCyan =		{ 0.45f, 0.6f, -1.0f };
		Vector3 vColourMagenta =	{ 0.45f, 0.4f, -1.0f };
		Vector3 vColourYellow =		{ 0.45f, 0.2f, -1.0f };
		Vector3 vColourReset =		{ 0.45f, 0.0f, -1.0f };

		Vector3 vTexture =			{ 0.45f, -0.5f, -1.0f };
		Vector3 vTexturePenguins =	{ 0.45f, -0.7f, -1.0f };
		Vector3 vTextureStars =		{ 0.45f, -0.9f, -1.0f };

		Vector3 vReturn =			{ -1.05f, -1.75f, -1.0f };

		Color cWhite =				{ 1.0f, 1.0f, 1.0f };
		Color cRed =				{ 1.0f, 0.2f, 0.2f };
		Color cGreen =				{ 0.0f, 1.0f, 0.0f };
		Color cBlue =				{ 0.2f, 0.2f, 1.0f };
		Color cCyan =				{ 0.0f, 1.0f, 1.0f };
		Color cMagenta =			{ 1.0f, 0.0f, 1.0f };
		Color cYellow =				{ 1.0f, 1.0f, 0.0f };
		Color cOrange =				{ 1.0f, 0.7f, 0.0f };
		Color cBrightGreen =		{ 0.1f, 0.8f, 0.4f };
		Color cLightGreen =			{ 0.5f, 1.0f, 0.6f };
		Color cPaleGreen =			{ 0.8f, 1.0f, 0.8f };

		DrawString("S T A R F I E L D    S C E N E    C O N T R O L S", &vTitle, &cBrightGreen);

		DrawString("c a m e r a", &vCamera, &cLightGreen);
		DrawString("`I' - Reset Position", &vCameraReset, &cWhite);
		DrawString("`UP' - Move Forward", &vCameraUp, &cWhite);
		DrawString("`DOWN' - Move Backwards", &vCameraDown, &cWhite);
		DrawString("`LEFT' - Tilt Left", &vCameraLeft, &cWhite);
		DrawString("`RIGHT' - Tilt Right", &vCameraRight, &cWhite);
		DrawString("c u b e    m o v e m e n t", &vMove, &cLightGreen);
		DrawString("`W' - Rotate Backward", &vMoveBackward, &cWhite);
		DrawString("`A' - Rotate Left", &vMoveLeft, &cWhite);
		DrawString("`S' - Rotate Forward", &vMoveForward, &cWhite);
		DrawString("`D' - Rotate Right", &vMoveRight, &cWhite);

		DrawString("`E' - Toggle Rotation", &vMoveRotation, &cWhite);
		DrawString("`Z' - Toggle Z Direction", &vMoveDirection, &cWhite);
		DrawString("`Q' - Toggle Movement", &vMoveMovement, &cWhite);

		DrawString("c u b e    c o l o u r", &vColour, &cLightGreen);
		DrawString("`F1' - Red", &vColourRed, &cWhite);
		DrawString("`F2' - Green", &vColourGreen, &cWhite);
		DrawString("`F3' - Blue", &vColourBlue, &cWhite);
		DrawString("`F4' - Cyan", &vColourCyan, &cWhite);
		DrawString("`F5' - Magenta", &vColourMagenta, &cWhite);
		DrawString("`F6' - Yellow", &vColourYellow, &cWhite);
		DrawString("`R' - Reset Colours", &vColourReset, &cWhite);

		DrawString("c u b e    t e x t u r e", &vTexture, &cLightGreen);
		DrawString("`P' - Penguin Texture", &vTexturePenguins, &cWhite);
		DrawString("`L' - Star Texture", &vTextureStars, &cWhite);

		DrawString("`T A B'    t o    r e t u r n    t o    t h e    s c e n e   . . .", &vReturn, &cPaleGreen);

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
}

void SceneStarfield::SceneAudio()
{
	if (paused)
	{
		if (!audioPlaying)
			PlaySound("Audio/mario_elevator.wav", GetModuleHandle(NULL), SND_LOOP | SND_ASYNC);
		
		audioPlaying = true;
	}
	else
	{
		if (audioPlaying)
			PlaySound(NULL, NULL, 0);

		if (colorAudio)
			PlaySound("Audio/button.wav", GetModuleHandle(NULL), SND_ASYNC);

		if (textureAudio)
			PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);

		audioPlaying = colorAudio = textureAudio = false;
	}
}

void SceneStarfield::CubeMovement(int iterator)
{
	if (zMoving)
	{
		if (!zReverse)
			objects[iterator]->_position.z += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		else
			objects[iterator]->_position.z -= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	srand(static_cast <unsigned> (time(0)));

	if (cRotating)
		objects[iterator]->_rotation += static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 7.5f));

	if (objects[iterator]->_position.z > 10.0f)
		objects[iterator]->_position.z = -100.0f;

	if (objects[iterator]->_position.z < -100.0f)
		objects[iterator]->_position.z = 10.0f;
}

void SceneStarfield::CameraMovement()
{
	if (!paused)
	{
		if (camera->eye.z < -75.0f)
		{
			camera->eye.z += MOVEMENT_SPEED;
			camera->center.z += MOVEMENT_SPEED;
			camera->up.z += MOVEMENT_SPEED;
		}
		else if (camera->eye.z > 30.0f)
		{
			camera->eye.z -= MOVEMENT_SPEED;
			camera->center.z -= MOVEMENT_SPEED;
			camera->up.z -= MOVEMENT_SPEED;
		}

		if (camera->eye.x > MOVEMENT_SPEED)
			camera->eye.x -= MOVEMENT_SPEED / 10;
		else if (camera->eye.x < -MOVEMENT_SPEED)
			camera->eye.x += MOVEMENT_SPEED / 10;
	}
}

void SceneStarfield::ResetCamera()
{
	camera->eye.x = 0.0f;
	camera->eye.y = 0.0f;
	camera->eye.z = 1.0f;

	camera->center.x = 0.0f;
	camera->center.y = 0.0f;
	camera->center.z = 0.0f;

	camera->up.x = 0.0f;
	camera->up.y = 1.0f;
	camera->up.z = 0.0f;
}