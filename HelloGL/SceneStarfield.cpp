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
		if (key == 'd')
			objects[i]->_rotation += 7.5f;

		if (key == 'a')
			objects[i]->_rotation -= 7.5f;

		if (key == 's')
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

		switch (key)
		{
		case 'r':
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 0.0f;

			colorAudio = colorIsRed = true;
			colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 'g':
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			
			colorAudio = colorIsGreen = true;
			colorIsRed = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 'b':
			objects[i]->red = 0.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;

			colorAudio = colorIsBlue = true;
			colorIsRed = colorIsGreen = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 'n':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;

			colorAudio = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 'c':
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;

			colorAudio = colorIsCyan = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsMagenta = colorIsYellow = false;
			break;

		case 'm':
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;

			colorAudio = colorIsMagenta = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsYellow = false;
			break;

		case 'y':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;

			colorAudio = colorIsYellow = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = false;
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
	}
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

	Vector3 vTitle = { -1.8f, 1.7f, camera->eye.z - 2.0f };
	Vector3 vReturn = { -0.7f, -1.75f, camera->eye.z - 2.0f };

	Color cWhite = { 1.0f, 1.0f, 1.0f };

	DrawString("Starfield Scene", &vTitle, &cWhite);
	DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);

	glEnable(GL_LIGHTING);

	Vector3 vPosition = { 0.5f, 1.7f, camera->eye.z - 2.0f };
	if (colorIsRed)
		DrawString("Colour changed to red.", &vPosition, &cWhite);
	else if (colorIsGreen)
		DrawString("Colour changed to green.", &vPosition, &cWhite);
	else if (colorIsBlue)
		DrawString("Colour changed to blue.", &vPosition, &cWhite);
	else if (colorIsCyan)
		DrawString("Colour changed to cyan.", &vPosition, &cWhite);
	else if (colorIsMagenta)
		DrawString("Colour changed to magenta.", &vPosition, &cWhite);
	else if (colorIsYellow)
		DrawString("Colour changed to yellow.", &vPosition, &cWhite);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}

void SceneStarfield::DrawMenu()
{
	if (paused)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		Vector3 vTitle = { -0.6f, 1.75f, -1.0f };

		Vector3 vCamera = { -1.7f, 1.25f, -1.0f };
		Vector3 vCameraReset = { -1.7f, 1.05f, -1.0f };
		Vector3 vCameraUp = { -1.7f, 0.85f, -1.0f };
		Vector3 vCameraDown = { -1.7f, 0.65f, -1.0f };
		Vector3 vCameraLeft = { -1.7f, 0.45f, -1.0f };
		Vector3 vCameraRight = { -1.7f, 0.25f, -1.0f };

		Vector3 vMR = { -1.7f, -0.1f, -1.0f };
		Vector3 vMRBackwards = { -1.7f, -0.3f, -1.0f };
		Vector3 vMRForwards = { -1.7f, -0.5f, -1.0f };
		Vector3 vMRRotation = { -1.7f, -0.7f, -1.0f };

		Vector3 vMRDirection = { -1.7f, -0.9f, -1.0f };
		Vector3 vMRMovement = { -1.7f, -1.1f, -1.0f };

		Vector3 vColour = { 0.25f, 1.25f, -1.0f };
		Vector3 vColourRed = { 0.25f, 1.05f, -1.0f };
		Vector3 vColourGreen = { 0.25f, 0.85f, -1.0f };
		Vector3 vColourBlue = { 0.25f, 0.65f, -1.0f };
		Vector3 vColourCyan = { 0.25f, 0.45f, -1.0f };
		Vector3 vColourMagenta = { 0.25f, 0.25f, -1.0f };
		Vector3 vColourYellow = { 0.25f, 0.05f, -1.0f };
		Vector3 vColourReset = { 0.25f, -0.15f, -1.0f };

		Vector3 vTexture = { 0.25f, -0.5f, -1.0f };
		Vector3 vTexturePenguins = { 0.25f, -0.7f, -1.0f };
		Vector3 vTextureStars = { 0.25f, -0.9f, -1.0f };

		Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

		Color cWhite = { 1.0f, 1.0f, 1.0f };
		Color cRed = { 1.0f, 0.2f, 0.2f };
		Color cGreen = { 0.0f, 1.0f, 0.0f };
		Color cBlue = { 0.2f, 0.2f, 1.0f };
		Color cCyan = { 0.0f, 1.0f, 1.0f };
		Color cMagenta = { 1.0f, 0.0f, 1.0f };
		Color cYellow = { 1.0f, 1.0f, 0.0f };
		Color cOrange = { 1.0f, 0.7f, 0.0f };

		DrawString("Starfield Scene Controls", &vTitle, &cRed);

		DrawString("Camera", &vCamera, &cYellow);
		DrawString("'i' - Reset camera position", &vCameraReset, &cWhite);
		DrawString("'up' - Move camera forward", &vCameraUp, &cWhite);
		DrawString("'down' - Move camera backwards", &vCameraDown, &cWhite);
		DrawString("'left' - Tilt camera left", &vCameraLeft, &cWhite);
		DrawString("'right' - Tilt camera right", &vCameraRight, &cWhite);

		DrawString("Movement & Rotation", &vMR, &cCyan);
		DrawString("'a' - Rotate cubes backwards", &vMRBackwards, &cWhite);
		DrawString("'d' - Rotate cubes forwards", &vMRForwards, &cWhite);
		DrawString("'e' - Toggle cube rotation", &vMRRotation, &cWhite);

		DrawString("'z' - Toggle z direction", &vMRDirection, &cWhite);
		DrawString("'q' - Toggle z movement", &vMRMovement, &cWhite);

		DrawString("Cube Colour", &vColour, &cOrange);
		DrawString("'r' - Change to red", &vColourRed, &cWhite);
		DrawString("'g' - Change to green", &vColourGreen, &cWhite);
		DrawString("'b' - Change to blue", &vColourBlue, &cWhite);
		DrawString("'c' - Change to cyan", &vColourCyan, &cWhite);
		DrawString("'m' - Change to magenta", &vColourMagenta, &cWhite);
		DrawString("'y' - Change to yellow", &vColourYellow, &cWhite);
		DrawString("'n' - Reset colours", &vColourReset, &cWhite);

		DrawString("Cube Texture", &vTexture, &cMagenta);
		DrawString("'p' - Change to penguin texture", &vTexturePenguins, &cWhite);
		DrawString("'s' - Change to star texture", &vTextureStars, &cWhite);

		DrawString("'TAB' to return to the scene.", &vReturn, &cRed);

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