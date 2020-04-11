#include "SceneOBJ.h"
#include <iostream>

SceneOBJ::SceneOBJ()
{
	InitGL();
	InitLighting();
	InitObjects();

	paused = false;
	audioPlaying = false;
	objectAudio = false;

	xPosition = 0.0f;
	yPosition = -1.0f;
	zPosition = -5.0f;

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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
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

	if (paused)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		Vector3 vTitle = { -0.6f, 1.75f, -1.0f };

		Vector3 vMovement = { -1.7f, 1.25f, -1.0f };
		Vector3 vMovementUp = { -1.7f, 1.05f, -1.0f };
		Vector3 vMovementLeft = { -1.7f, 0.85f, -1.0f };
		Vector3 vMovementDown = { -1.7f, 0.65f, -1.0f };
		Vector3 vMovementRight = { -1.7f, 0.45f, -1.0f };
		Vector3 vMovementBack = { -1.7f, 0.25f, -1.0f };
		Vector3 vMovementForward = { -1.7f, 0.05f, -1.0f };

		Vector3 vObject = { 0.25f, 1.25f, -1.0f };
		Vector3 vObjectTank = { 0.25f, 1.05f, -1.0f };
		Vector3 vObjectSkull = { 0.25f, 0.85f, -1.0f };
		Vector3 vObjectCube = { 0.25f, 0.65f, -1.0f };

		Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

		Color cWhite = { 1.0f, 1.0f, 1.0f };
		Color cRed = { 1.0f, 0.2f, 0.2f };
		Color cGreen = { 0.0f, 1.0f, 0.0f };
		Color cBlue = { 0.2f, 0.2f, 1.0f };
		Color cCyan = { 0.0f, 1.0f, 1.0f };
		Color cMagenta = { 1.0f, 0.0f, 1.0f };
		Color cYellow = { 1.0f, 1.0f, 0.0f };
		Color cOrange = { 1.0f, 0.7f, 0.0f };

		DrawString("OBJ Loader Controls", &vTitle, &cRed);

		DrawString("Object Movement", &vMovement, &cYellow);
		DrawString("'w' - Move object up", &vMovementUp, &cWhite);
		DrawString("'a' - Move object left", &vMovementLeft, &cWhite);
		DrawString("'s' - Move object down", &vMovementDown, &cWhite);
		DrawString("'d' - Move object right", &vMovementRight, &cWhite);
		DrawString("'up' - Move object backward", &vMovementBack, &cWhite);
		DrawString("'down' - Move object forward", &vMovementForward, &cWhite);

		DrawString("Change OBJ", &vObject, &cOrange);
		DrawString("'t' - Change to tank", &vObjectTank, &cWhite);
		DrawString("'h' - Change to skull", &vObjectSkull, &cWhite);
		DrawString("'c' - Change to cube", &vObjectCube, &cWhite);

		DrawString("'TAB' to return to the scene.", &vReturn, &cRed);

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);

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

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

			Vector3 vTitle = { -1.8f, 1.7f, -1.0f };
			Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

			Color cWhite = { 1.0f, 1.0f, 1.0f };

			DrawString("OBJ Loader", &vTitle, &cWhite);
			DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}

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

	if (paused)
	{
		if (!audioPlaying)
		{
			audioPlaying = true;
			PlaySound("Audio/mario_elevator.wav", GetModuleHandle(NULL), SND_LOOP | SND_ASYNC);
		}
	}
	else
	{
		if (audioPlaying)
		{
			audioPlaying = false;
			PlaySound(NULL, NULL, 0);
		}
		if (objectAudio)
		{
			objectAudio = false;
			PlaySound("Audio/button.wav", GetModuleHandle(NULL), SND_ASYNC);
		}
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void SceneOBJ::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		if (tankLoaded)
			yPosition += 0.2f;
		else if (cubeLoaded)
			yPosition += 0.1f;
		else if (skullLoaded)
			yPosition += 2.0f;
	}
	if (key == 'a')
	{
		if (tankLoaded)
			xPosition -= 0.2f;
		else if (cubeLoaded)
			xPosition -= 0.1f;
		else if (skullLoaded)
			xPosition -= 2.0f;
	}
	if (key == 's')
	{
		if (tankLoaded)
			yPosition -= 0.2f;
		else if (cubeLoaded)
			yPosition -= 0.1f;
		else if (skullLoaded)
			yPosition -= 2.0f;
	}
	if (key == 'd')
	{
		if (tankLoaded)
			xPosition += 0.2f;
		else if (cubeLoaded)
			xPosition += 0.1f;
		else if (skullLoaded)
			xPosition += 2.0f;
	}

	if (key == 't')
	{
		tankLoaded = true;
		cubeLoaded = false;
		skullLoaded = false;

		objectAudio = true;

		xPosition = 0.0f;
		yPosition = -1.0f;
		zPosition = -5.0f;
	}
	if (key == 'c')
	{
		tankLoaded = false;
		cubeLoaded = true;
		skullLoaded = false;

		objectAudio = true;

		xPosition = 0.0f;
		yPosition = -0.25f;
		zPosition = -0.5f;
	}
	if (key == 'h')
	{
		tankLoaded = false;
		cubeLoaded = false;
		skullLoaded = true;

		objectAudio = true;

		xPosition = 0.0f;
		yPosition = -10.0f;
		zPosition = -30.0f;
	}

	if (key == 9)
	{
		if (!paused)
			paused = true;
		else
			paused = false;
	}
}

void SceneOBJ::KeyboardSpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		if (tankLoaded)
			zPosition -= 0.2f;
		else if (cubeLoaded)
			zPosition -= 0.1f;
		else if (skullLoaded)
			zPosition -= 2.0f;
	}
	if (key == GLUT_KEY_DOWN)
	{
		if (tankLoaded)
			zPosition += 0.2f;
		else if (cubeLoaded)
			zPosition += 0.1f;
		else if (skullLoaded)
			zPosition += 2.0f;
	}
}

void SceneOBJ::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glColor3f(color->r, color->g, color->b);
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text);
	glPopMatrix();
}