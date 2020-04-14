#include "SceneOBJ.h"

SceneOBJ::SceneOBJ()
{
	InitGL();
	InitLighting();
	InitObjects();

	tankLoaded = true;
	xPosition = 0.0f;
	yPosition = -1.0f;
	zPosition = -5.0f;

	glutMainLoop();
}

SceneOBJ::~SceneOBJ(void)
{

}

void SceneOBJ::InitGL()
{
	Scene::InitGL();
	GLUTCallbacks::Init(this);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
}

void SceneOBJ::InitLighting()
{
	Scene::InitLighting();
}

void SceneOBJ::InitObjects()
{
	Scene::InitObjects();
	tankObj.Load((char*)"Objects/IS7.obj");
	cubeObj.Load((char*)"Objects/cube.obj");
	skullObj.Load((char*)"Objects/skull.obj");
}

void SceneOBJ::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawMenu();

	if (!paused)
	{
		glDisable(GL_TEXTURE_2D);
		LoadOBJ();
		DrawUI();
		glEnable(GL_TEXTURE_2D);
	}

	glFlush();
	glutSwapBuffers();
}

void SceneOBJ::Update()
{
	glLoadIdentity();
	
	Scene::Update();
	SceneAudio();

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
		tankLoaded = objectAudio = true;
		cubeLoaded = skullLoaded = false;

		xPosition = 0.0f;
		yPosition = -1.0f;
		zPosition = -5.0f;
	}

	if (key == 'c')
	{
		cubeLoaded = objectAudio = true;
		tankLoaded = skullLoaded = false;

		xPosition = 0.0f;
		yPosition = -0.25f;
		zPosition = -0.5f;
	}

	if (key == 'h')
	{
		skullLoaded = objectAudio = true;
		tankLoaded = cubeLoaded = false;

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

void SceneOBJ::DrawUI()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Vector3 vTitle = { -1.8f, 1.7f, -1.0f };
	Vector3 vReturn = { -0.7f, -1.75f, -1.0f };

	Color cWhite = { 1.0f, 1.0f, 1.0f };

	DrawString("OBJ Loader", &vTitle, &cWhite);
	DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void SceneOBJ::DrawMenu()
{
	
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
}

void SceneOBJ::SceneAudio()
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

		if (objectAudio)
			PlaySound("Audio/button.wav", GetModuleHandle(NULL), SND_ASYNC);

		audioPlaying = objectAudio = false;
	}
}

void SceneOBJ::LoadOBJ()
{
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
}