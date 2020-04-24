#include "SceneCollision.h"
#include "MeshLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneCollision::SceneCollision() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();
	InitMenu();

	SetCubePositions();
	collisionTimer = COLLISION_TIMER;

	glutMainLoop();
}

SceneCollision::~SceneCollision(void)
{	
	Scene::~Scene();
	
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
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
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

void SceneCollision::InitMenu()
{
	colourMenu = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddMenuEntry("Red", 0);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Cyan", 3);
	glutAddMenuEntry("Magenta", 4);
	glutAddMenuEntry("Yellow", 5);

	helpMenu = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddMenuEntry("Controls", 6);
	glutAddMenuEntry("Exit", 7);
	
	subMenus = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddSubMenu("Colour", colourMenu);
	glutAddSubMenu("Help", helpMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	switch (key)
	{
	case 'i':
	case 'I':
		SetCubePositions();
		PlaySound("Audio/hint.wav", GetModuleHandle(NULL), SND_ASYNC);
		break;

	case 9:
		if (!paused)
			paused = true;
		else
			paused = false;
		break;

	default:
		break;
	}

	if (collisionTimer == COLLISION_TIMER)
	{
		switch (key)
		{
		case 'w':
		case 'W':
			penguinMovingUp = true;
			break;

		case 's':
		case 'S':
			penguinMovingDown = true;
			break;

		case 'a':
		case 'A':
			penguinMovingLeft = true;
			break;

		case 'd':
		case 'D':
			penguinMovingRight = true;
			break;

		default:
			break;
		}
	}

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		switch (key)
		{
		case 'r':
		case 'R':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;
			colorAudio = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		default:
			break;
		}
	}
}

void SceneCollision::KeyboardSpecial(int key, int x, int y)
{
	if (collisionTimer == COLLISION_TIMER)
	{
		switch (key)
		{
		case GLUT_KEY_UP:
			starMovingUp = true;
			break;

		case GLUT_KEY_DOWN:
			starMovingDown = true;
			break;

		case GLUT_KEY_LEFT:
			starMovingLeft = true;
			break;

		case GLUT_KEY_RIGHT:
			starMovingRight = true;
			break;

		default:
			break;
		}
	}

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

		default:
			break;
		}
	}
}

void SceneCollision::KeyboardUp(unsigned char key, int x, int y)
{
	cubeSpeed = 1.0f;

	switch (key)
	{
	case 'w':
	case 'W':
		penguinMovingUp = false;
		break;

	case 's':
	case 'S':
		penguinMovingDown = false;
		break;

	case 'a':
	case 'A':
		penguinMovingLeft = false;
		break;

	case 'd':
	case 'D':
		penguinMovingRight = false;
		break;

	default:
		break;
	}
}

void SceneCollision::KeyboardSpecialUp(int key, int x, int y)
{
	cubeSpeed = 1.0f;

	switch (key)
	{
	case GLUT_KEY_UP:
		starMovingUp = false;
		break;

	case GLUT_KEY_DOWN:
		starMovingDown = false;
		break;

	case GLUT_KEY_LEFT:
		starMovingLeft = false;
		break;

	case GLUT_KEY_RIGHT:
		starMovingRight = false;
		break;

	default:
		break;
	}
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

		Vector3 vTitle =		 { -1.0f, 1.75f, -1.0f };

		Vector3 vPenguin =		 { -1.5f, 1.2f, -1.0f };
		Vector3 vPenguinW =		 { -1.5f, 1.0f, -1.0f };
		Vector3 vPenguinA =		 { -1.5f, 0.8f, -1.0f };
		Vector3 vPenguinS =		 { -1.5f, 0.6f, -1.0f };
		Vector3 vPenguinD =		 { -1.5f, 0.4f, -1.0f };

		Vector3 vStar =			 { -1.5f, -0.1f, -1.0f };
		Vector3 vStarUp =		 { -1.5f, -0.3f, -1.0f };
		Vector3 vStarLeft =		 { -1.5f, -0.5f, -1.0f };
		Vector3 vStarDown =		 { -1.5f, -0.7f, -1.0f };
		Vector3 vStarRight =	 { -1.5f, -0.9f, -1.0f };

		Vector3 vColour =		 { 0.45f, 1.2f, -1.0f };
		Vector3 vColourRed =	 { 0.45f, 1.0f, -1.0f };
		Vector3 vColourGreen =	 { 0.45f, 0.8f, -1.0f };
		Vector3 vColourBlue =	 { 0.45f, 0.6f, -1.0f };
		Vector3 vColourCyan =	 { 0.45f, 0.4f, -1.0f };
		Vector3 vColourMagenta = { 0.45f, 0.2f, -1.0f };
		Vector3 vColourYellow =  { 0.45f, 0.0f, -1.0f };
		Vector3 vColourReset =	 { 0.45f, -0.2f, -1.0f };
								 
		Vector3 vCubes =		 { 0.45f, -0.7f, -1.0f };
		Vector3 vCubesReset =	 { 0.45f, -0.9f, -1.0f };
								 
		Vector3 vReturn =		 { -1.05f, -1.75f, -1.0f };

		Color cWhite =		{ 1.0f, 1.0f, 1.0f };
		Color cSea =		{ 0.2f, 0.5f, 1.0f };
		Color cDarkCyan =	{ 0.2f, 0.8f, 1.0f };
		Color cLightBlue =	{ 0.4f, 1.0f, 1.0f };

		DrawString("C O L L I S I O N    S C E N E    C O N T R O L S", &vTitle, &cSea);

		DrawString("p e n g u i n    c u b e", &vPenguin, &cDarkCyan);
		DrawString("`W' - Move Up", &vPenguinW, &cWhite);
		DrawString("`A' - Move Left", &vPenguinA, &cWhite);
		DrawString("`S' - Move Down", &vPenguinS, &cWhite);
		DrawString("`D' - Move Right", &vPenguinD, &cWhite);

		DrawString("s t a r    c u b e", &vStar, &cDarkCyan);
		DrawString("`UP' - Move Up", &vStarUp, &cWhite);
		DrawString("`LEFT' - Move Left", &vStarLeft, &cWhite);
		DrawString("`DOWN' - Move Down", &vStarDown, &cWhite);
		DrawString("`RIGHT' - Move Right", &vStarRight, &cWhite);

		DrawString("c u b e    c o l o u r", &vColour, &cDarkCyan);
		DrawString("`F1' - Red", &vColourRed, &cWhite);
		DrawString("`F2' - Green", &vColourGreen, &cWhite);
		DrawString("`F3' - Blue", &vColourBlue, &cWhite);
		DrawString("`F4' - Cyan", &vColourCyan, &cWhite);
		DrawString("`F5' - Magenta", &vColourMagenta, &cWhite);
		DrawString("`F6' - Yellow", &vColourYellow, &cWhite);
		DrawString("`R' - Reset Colours", &vColourReset, &cWhite);
		
		DrawString("c u b e    p o s i t i o n s", &vCubes, &cDarkCyan);
		DrawString("`I' - Reset Cube Positions", &vCubesReset, &cWhite);

		DrawString("`T A B'    t o    r e t u r n    t o    t h e    s c e n e   . . .", &vReturn, &cLightBlue);

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

		Vector3 vTitle =		{ -1.8f, 1.7f, -1.0f };
		Vector3 vReturn =		{ -1.05f, -1.75f, -1.0f };
		Vector3 vCollision =	{ 0.6f, 1.7f, -1.0f };
		Vector3 vPosition =		{ 0.6f, 1.5f, -1.0f };

		Color cWhite =		{ 1.0f, 1.0f, 1.0f };
		Color cSea =		{ 0.2f, 0.5f, 1.0f };
		Color cDarkCyan =	{ 0.2f, 0.8f, 1.0f };
		Color cLightBlue =	{ 0.4f, 1.0f, 1.0f };

		DrawString("C O L L I S I O N    S C E N E", &vTitle, &cSea);
		DrawString("`T A B'    t o    v i e w    s c e n e    c o n t r o l s    . . .", &vReturn, &cLightBlue);

		if (cubeCollision)
			DrawString("Cube Collision Occured", &vCollision, &cWhite);

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

void SceneCollision::MouseMenu(int value)
{
	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		switch (value)
		{
		case 0:
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 0.0f;
			colorAudio = colorIsRed = true;
			colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 1:
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			colorAudio = colorIsGreen = true;
			colorIsRed = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 2:
			objects[i]->red = 0.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;
			colorAudio = colorIsBlue = true;
			colorIsRed = colorIsGreen = colorIsCyan = colorIsMagenta = colorIsYellow = false;
			break;

		case 3:
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;
			colorAudio = colorIsCyan = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsMagenta = colorIsYellow = false;
			break;

		case 4:
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;
			colorAudio = colorIsMagenta = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsYellow = false;
			break;

		case 5:
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			colorAudio = colorIsYellow = true;
			colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = false;
			break;

		default:
			break;
		}
	}

	switch (value)
	{
	case 6:
		if (!paused)
			paused = true;
		else
			paused = false;
		break;

	case 7:
		glutDestroyWindow(glutGetWindow());
		exit(0);
		break;

	default:
		break;
	}
}