#include "SceneCollision.h"
#include "MeshLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

SceneCollision::SceneCollision() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	SetCubePositions();
	collisionTimer = COLLISION_TIMER;

	glutMainLoop();
}

SceneCollision::~SceneCollision(void)
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

void SceneCollision::InitGL()
{
	Scene::InitGL();
	GLUTCallbacks::Init(this);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutSpecialUpFunc(GLUTCallbacks::KeyboardSpecialUp);
}

void SceneCollision::InitLighting()
{
	Scene::InitLighting();
}

void SceneCollision::InitObjects()
{
	Scene::InitObjects();

	cubeMesh = MeshLoader::Load((char*)"Objects/cube.txt");

	texturePenguins = new Texture2D();
	texturePenguins->Load((char*)"Textures/penguins.raw", 512, 512);

	textureStars = new Texture2D();
	textureStars->Load((char*)"Textures/stars.raw", 512, 512);

	objects[0] = new Cube(cubeMesh, texturePenguins, 0.0f, 0.0f, 0.0f);
	objects[1] = new Cube(cubeMesh, textureStars, 0.0f, 0.0f, 0.0f);
}

void SceneCollision::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawMenu();
	DrawUI();

	if (!paused)
	{
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->Draw();
		}
		CubeCollision();
	}

	glFlush();
	glutSwapBuffers();
}

void SceneCollision::Update()
{
	glLoadIdentity();
	
	Scene::Update();
	SceneAudio();

	if (!paused)
	{
		if (collisionTimer == COLLISION_TIMER)
		{
			MovePenguinCube();
			MoveStarCube();
		}
		for (int i = 0; i < OBJECTCOUNT; i++)
		{
			objects[i]->Update();
			CubeScreenCollision(i);
		}
		UpdateCubePositions();
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void SceneCollision::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'i')
		SetCubePositions();

	if (collisionTimer == COLLISION_TIMER)
	{
		if (key == 'w')
			penguinMovingUp = true;

		if (key == 's')
			penguinMovingDown = true;

		if (key == 'a')
			penguinMovingLeft = true;

		if (key == 'd')
			penguinMovingRight = true;
	}

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
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

void SceneCollision::KeyboardSpecial(int key, int x, int y)
{
	if (collisionTimer == COLLISION_TIMER)
	{
		if (key == GLUT_KEY_UP)
			starMovingUp = true;

		if (key == GLUT_KEY_DOWN)
			starMovingDown = true;

		if (key == GLUT_KEY_LEFT)
			starMovingLeft = true;

		if (key == GLUT_KEY_RIGHT)
			starMovingRight = true;
	}
}

void SceneCollision::KeyboardUp(unsigned char key, int x, int y)
{
	cubeSpeed = 1.0f;
	
	if (key == 'w')
		penguinMovingUp = false;

	if (key == 's')
		penguinMovingDown = false;

	if (key == 'a')
		penguinMovingLeft = false;

	if (key == 'd')
		penguinMovingRight = false;
}

void SceneCollision::KeyboardSpecialUp(int key, int x, int y)
{
	cubeSpeed = 1.0f;

	if (key == GLUT_KEY_UP)
		starMovingUp = false;

	if (key == GLUT_KEY_DOWN)
		starMovingDown = false;

	if (key == GLUT_KEY_LEFT)
		starMovingLeft = false;

	if (key == GLUT_KEY_RIGHT)
		starMovingRight = false;
}

void SceneCollision::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glColor3f(color->r, color->g, color->b);
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
	glPopMatrix();
}

void SceneCollision::DrawMenu()
{
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

		Vector3 vStar = { -1.7f, 0.1f, -1.0f };
		Vector3 vStarUp = { -1.7f, -0.1f, -1.0f };
		Vector3 vStarLeft = { -1.7f, -0.3f, -1.0f };
		Vector3 vStarDown = { -1.7f, -0.5f, -1.0f };
		Vector3 vStarRight = { -1.7f, -0.7f, -1.0f };

		Vector3 vColour = { 0.25f, 1.25f, -1.0f };
		Vector3 vColourRed = { 0.25f, 1.05f, -1.0f };
		Vector3 vColourGreen = { 0.25f, 0.85f, -1.0f };
		Vector3 vColourBlue = { 0.25f, 0.65f, -1.0f };
		Vector3 vColourCyan = { 0.25f, 0.45f, -1.0f };
		Vector3 vColourMagenta = { 0.25f, 0.25f, -1.0f };
		Vector3 vColourYellow = { 0.25f, 0.05f, -1.0f };
		Vector3 vColourReset = { 0.25f, -0.15f, -1.0f };
		
		Vector3 vCubes = { 0.25f, -0.50f, -1.0f };
		Vector3 vCubesReset = { 0.25f, -0.70f, -1.0f };

		Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

		Color cWhite = { 1.0f, 1.0f, 1.0f };
		Color cRed = { 1.0f, 0.2f, 0.2f };
		Color cGreen = { 0.0f, 1.0f, 0.0f };
		Color cBlue = { 0.2f, 0.2f, 1.0f };
		Color cCyan = { 0.0f, 1.0f, 1.0f };
		Color cMagenta = { 1.0f, 0.0f, 1.0f };
		Color cYellow = { 1.0f, 1.0f, 0.0f };
		Color cOrange = { 1.0f, 0.7f, 0.0f };

		DrawString("Collision Scene Controls", &vTitle, &cRed);

		DrawString("Penguin Cube", &vPenguin, &cYellow);
		DrawString("'w' - Move cube up", &vPenguinW, &cWhite);
		DrawString("'a' - Move cube left", &vPenguinA, &cWhite);
		DrawString("'s' - Move cube down", &vPenguinS, &cWhite);
		DrawString("'d' - Move cube right", &vPenguinD, &cWhite);

		DrawString("Star Cube", &vStar, &cCyan);
		DrawString("'up' - Move cube up", &vStarUp, &cWhite);
		DrawString("'left' - Move cube left", &vStarLeft, &cWhite);
		DrawString("'down' - Move cube down", &vStarDown, &cWhite);
		DrawString("'right' - Move cube right", &vStarRight, &cWhite);

		DrawString("Cube Colour", &vColour, &cOrange);
		DrawString("'r' - Change to red", &vColourRed, &cWhite);
		DrawString("'g' - Change to green", &vColourGreen, &cWhite);
		DrawString("'b' - Change to blue", &vColourBlue, &cWhite);
		DrawString("'c' - Change to cyan", &vColourCyan, &cWhite);
		DrawString("'m' - Change to magenta", &vColourMagenta, &cWhite);
		DrawString("'y' - Change to yellow", &vColourYellow, &cWhite);
		DrawString("'n' - Reset colours", &vColourReset, &cWhite);
		
		DrawString("Scene Cubes", &vCubes, &cMagenta);
		DrawString("'i' - Reset all cube positions", &vCubesReset, &cWhite);

		DrawString("'TAB' to return to the scene.", &vReturn, &cRed);

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
}

void SceneCollision::DrawUI()
{
	if (!paused)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
	
		if (cubeCollision)
		{
			Vector3 vPosition = { 0.6f, 1.7f, -1.0f };
			Color cWhite = { 1.0f, 1.0f, 1.0f };
			DrawString("Cube collision occured.", &vPosition, &cWhite);
		}

		Vector3 vTitle = { -1.8f, 1.7f, -1.0f };
		Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

		Color cWhite = { 1.0f, 1.0f, 1.0f };

		DrawString("Collision Example", &vTitle, &cWhite);
		DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);

		glEnable(GL_LIGHTING);

		Vector3 vPosition = { 0.6f, 1.5f, -1.0f };
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
}

void SceneCollision::SceneAudio()
{
	if (!paused)
	{
		if (audioPlaying)
			PlaySound(NULL, NULL, 0);

		if (cubeAudio)
			PlaySound("Audio/menu_click.wav", GetModuleHandle(NULL), SND_ASYNC);

		if (colorAudio)
			PlaySound("Audio/button.wav", GetModuleHandle(NULL), SND_ASYNC);

		audioPlaying = cubeAudio = colorAudio = false;
	}
	else
	{
		if (!audioPlaying)
			PlaySound("Audio/mario_elevator.wav", GetModuleHandle(NULL), SND_LOOP | SND_ASYNC);
		
		audioPlaying = true;
	}	
}

void SceneCollision::CubeCollision()
{
	float distance = CalculateDistanceSquared(objects[0], objects[1]);

	bool distanceSquared = true;
	float radiusDistance;

	if (distanceSquared)
		radiusDistance = pow(objects[0]->_radius + objects[1]->_radius, 2);
	else
		radiusDistance = objects[0]->_radius + objects[1]->_radius;

	if (distance <= radiusDistance)
	{
		cubeCollision = true;
		cubeAudio = true;

		if (objects[0]->_position.x < objects[1]->_position.x)
			innerCollision = true;
		
		if (objects[0]->_position.x > objects[1]->_position.x)
			outerCollision = true;

		if (objects[0]->_position.y < objects[1]->_position.y && ((objects[0]->_position.x + 1 > objects[1]->_position.x) || (objects[0]->_position.x < objects[1]->_position.x - 1)))
			upperCollision = true;
		
		if (objects[0]->_position.y > objects[1]->_position.y && ((objects[0]->_position.x + 1 < objects[1]->_position.x) || (objects[0]->_position.x > objects[1]->_position.x - 1)))
			lowerCollision = true;
	}
}

float SceneCollision::CalculateDistanceSquared(SceneObject* c1, SceneObject* c2)
{
	float distance = ((c1->_position.x - c2->_position.x) * (c1->_position.x - c2->_position.x)) +
					 ((c1->_position.y - c2->_position.y) * (c1->_position.y - c2->_position.y)) +
					 ((c1->_position.z - c2->_position.z) * (c1->_position.z - c2->_position.z));
	return distance;
}

void SceneCollision::CubeScreenCollision(int iterator)
{
	if (objects[iterator]->_position.x > 4.0f)
		objects[iterator]->_position.x = 4.0f;

	if (objects[iterator]->_position.x < -4.0f)
		objects[iterator]->_position.x = -4.0f;

	if (objects[iterator]->_position.y > 4.0f)
		objects[iterator]->_position.y = 4.0f;

	if (objects[iterator]->_position.y < -4.0f)
		objects[iterator]->_position.y = -4.0f;
}

void SceneCollision::MovePenguinCube()
{
	if (penguinMovingUp)
		objects[0]->_position.y += CUBE_MOVEMENT * cubeSpeed;
	else if (penguinMovingDown)
		objects[0]->_position.y -= CUBE_MOVEMENT * cubeSpeed;
	else if (penguinMovingLeft)
		objects[0]->_position.x -= CUBE_MOVEMENT * cubeSpeed;
	else if (penguinMovingRight)
		objects[0]->_position.x += CUBE_MOVEMENT * cubeSpeed;

	if (penguinMovingUp || penguinMovingDown || penguinMovingLeft || penguinMovingRight)
		cubeSpeed += 0.1f;
}

void SceneCollision::MoveStarCube()
{
	if (starMovingUp)
		objects[1]->_position.y += CUBE_MOVEMENT * cubeSpeed;
	else if (starMovingDown)
		objects[1]->_position.y -= CUBE_MOVEMENT * cubeSpeed;
	else if (starMovingLeft)
		objects[1]->_position.x -= CUBE_MOVEMENT * cubeSpeed;
	else if (starMovingRight)
		objects[1]->_position.x += CUBE_MOVEMENT * cubeSpeed;

	if (starMovingUp || starMovingDown || starMovingLeft || starMovingRight)
		cubeSpeed += 0.1f;
}

void SceneCollision::SetCubePositions()
{
	objects[0]->_position.x = -2.5f;
	objects[0]->_position.y = 0.0f;
	objects[0]->_position.z = -5.0f;

	objects[1]->_position.x = 2.5f;
	objects[1]->_position.y = 0.0f;
	objects[1]->_position.z = -5.0f;
}

void SceneCollision::UpdateCubePositions()
{
	if (innerCollision)
	{	
		objects[0]->_position.x -= CUBE_MOVEMENT * cubeSpeed;
		objects[1]->_position.x += CUBE_MOVEMENT * cubeSpeed;
	}

	if (outerCollision)
	{
		objects[0]->_position.x += CUBE_MOVEMENT * cubeSpeed;
		objects[1]->_position.x -= CUBE_MOVEMENT * cubeSpeed;
	}

	if (upperCollision)
	{
		objects[0]->_position.y -= CUBE_MOVEMENT * cubeSpeed;
		objects[1]->_position.y += CUBE_MOVEMENT * cubeSpeed;
	}

	if (lowerCollision)
	{
		objects[0]->_position.y += CUBE_MOVEMENT * cubeSpeed;
		objects[1]->_position.y -= CUBE_MOVEMENT * cubeSpeed;
	}

	if (innerCollision || outerCollision || upperCollision || lowerCollision)
	{
		collisionTimer -= 1;
		cubeSpeed -= 0.1f;

		if (collisionTimer < 0)
		{
			cubeSpeed = 0.0f;
			collisionTimer = COLLISION_TIMER;
			innerCollision = outerCollision = upperCollision = lowerCollision = false;
		}
	}
}