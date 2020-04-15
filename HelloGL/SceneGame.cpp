#include "SceneGame.h"

SceneGame::SceneGame() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();
	glutMainLoop();
}

SceneGame::~SceneGame(void)
{

}

void SceneGame::InitGL()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, 1.0f, 0.1f, 1000);
	glMatrixMode(GL_MODELVIEW);
	
	GLUTCallbacks::Init(this);
	glutDisplayFunc(GLUTCallbacks::Display);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
}

void SceneGame::InitLighting()
{
	Scene::InitLighting();
}

void SceneGame::InitObjects()
{
	Scene::InitObjects();
	cubeXBlue = 19;
}

void SceneGame::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	if (paused)
		DrawMenu();
	else
	{
		glTranslatef(-13, 3.5, -46);
		glRotatef(40, 1, 1, 0);

		DrawGrid();
		DrawQuad();
		DrawCubes();

		DrawUI();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glFlush();
	glutSwapBuffers();
}

void SceneGame::Update()
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

void SceneGame::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w' && cubeZRed > 0)
		cubeZRed -= 1;

	if (key == 's' && cubeZRed < 19)
		cubeZRed += 1;

	if (key == 'a' && cubeXRed > 0)
		cubeXRed -= 1;

	if (key == 'd' && cubeXRed < 19)
		cubeXRed += 1;

	if (key == 'q' && cubeY < 4)
		cubeY += 1;

	if (key == 'e' && cubeY > -4)
		cubeY -= 1;

	if (key == 'n')
	{
		for (int i = 0; i < Q_Red[0].total + 1; i++)
		{
			Q_Red[i].x1 = 0;
			Q_Red[i].x2 = 0;
			Q_Red[i].x3 = 0;
			Q_Red[i].x4 = 0;
			
			Q_Red[i].y1 = 0;
			Q_Red[i].y2 = 0;
			Q_Red[i].y3 = 0;
			Q_Red[i].y4 = 0;
			
			Q_Red[i].z1 = 0;
			Q_Red[i].z2 = 0;
			Q_Red[i].z3 = 0;
			Q_Red[i].z4 = 0;
		}
		for (int i = 0; i < Q_Blue[0].total + 1; i++)
		{
			Q_Blue[i].x1 = 0;
			Q_Blue[i].x2 = 0;
			Q_Blue[i].x3 = 0;
			Q_Blue[i].x4 = 0;

			Q_Blue[i].y1 = 0;
			Q_Blue[i].y2 = 0;
			Q_Blue[i].y3 = 0;
			Q_Blue[i].y4 = 0;

			Q_Blue[i].z1 = 0;
			Q_Blue[i].z2 = 0;
			Q_Blue[i].z3 = 0;
			Q_Blue[i].z4 = 0;
		}
		PlaySound("Audio/hint.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (key == 32)
	{
		AddQuadRed();
		cubeRed = true;
	}

	if (key == 13)
	{
		AddQuadBlue();
		cubeBlue = true;
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
	if (key == GLUT_KEY_UP && cubeZBlue > 0)
		cubeZBlue -= 1;

	if (key == GLUT_KEY_DOWN && cubeZBlue < 19)
		cubeZBlue += 1;

	if (key == GLUT_KEY_LEFT && cubeXBlue > 0)
		cubeXBlue -= 1;

	if (key == GLUT_KEY_RIGHT && cubeXBlue < 19)
		cubeXBlue += 1;

	if (key == GLUT_KEY_F1)
	{
		Q_Red[cubeN].r = 1;
		Q_Red[cubeN].g = 0;
		Q_Red[cubeN].b = 0;
	}

	if (key == GLUT_KEY_F2)
	{
		Q_Red[cubeN].r = 0;
		Q_Red[cubeN].g = 1;
		Q_Red[cubeN].b = 0;
	}

	if (key == GLUT_KEY_F3)
	{
		Q_Red[cubeN].r = 0;
		Q_Red[cubeN].g = 0;
		Q_Red[cubeN].b = 1;
	}

	if (key == GLUT_KEY_F4)
	{
		Q_Red[cubeN].r = 0;
		Q_Red[cubeN].g = 1;
		Q_Red[cubeN].b = 1;
	}

	if (key == GLUT_KEY_F5)
	{
		Q_Red[cubeN].r = 1;
		Q_Red[cubeN].g = 0;
		Q_Red[cubeN].b = 1;
	}

	if (key == GLUT_KEY_F6)
	{
		Q_Red[cubeN].r = 1;
		Q_Red[cubeN].g = 1;
		Q_Red[cubeN].b = 0;
	}

	if (key == GLUT_KEY_F7)
	{
		Q_Blue[cubeN].r = 1;
		Q_Blue[cubeN].g = 0;
		Q_Blue[cubeN].b = 0;
	}

	if (key == GLUT_KEY_F8)
	{
		Q_Blue[cubeN].r = 0;
		Q_Blue[cubeN].g = 1;
		Q_Blue[cubeN].b = 0;
	}

	if (key == GLUT_KEY_F9)
	{
		Q_Blue[cubeN].r = 0;
		Q_Blue[cubeN].g = 0;
		Q_Blue[cubeN].b = 1;
	}

	if (key == GLUT_KEY_F10)
	{
		Q_Blue[cubeN].r = 0;
		Q_Blue[cubeN].g = 1;
		Q_Blue[cubeN].b = 1;
	}

	if (key == GLUT_KEY_F11)
	{
		Q_Blue[cubeN].r = 1;
		Q_Blue[cubeN].g = 0;
		Q_Blue[cubeN].b = 1;
	}

	if (key == GLUT_KEY_F12)
	{
		Q_Blue[cubeN].r = 1;
		Q_Blue[cubeN].g = 1;
		Q_Blue[cubeN].b = 0;
	}

	if (key == GLUT_KEY_F1 ||
		key == GLUT_KEY_F2 ||
		key == GLUT_KEY_F3 ||
		key == GLUT_KEY_F4 ||
		key == GLUT_KEY_F5 ||
		key == GLUT_KEY_F6 ||
		key == GLUT_KEY_F7 ||
		key == GLUT_KEY_F8 ||
		key == GLUT_KEY_F9 ||
		key == GLUT_KEY_F10 ||
		key == GLUT_KEY_F11 ||
		key == GLUT_KEY_F12)
		colourAudio = true;
}

void SceneGame::KeyboardUp(unsigned char key, int x, int y)
{
	if (key == 32)
		cubeRed = false;

	if (key == 13)
		cubeBlue = false;
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

void SceneGame::DrawUI()
{
	glDisable(GL_DEPTH_TEST);

	Vector3 vTitle = { 0.25, 8.575f, -2.0f };
	Vector3 vReturn = { 11.15f, -27.05f, -2.0f };

	Color cWhite = { 1.0f, 1.0f, 1.0f };

	DrawString("OpenGL Game", &vTitle, &cWhite);
	DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);

	glEnable(GL_DEPTH_TEST);
}

void SceneGame::DrawMenu()
{
	glDisable(GL_DEPTH_TEST);

	Vector3 vTitle = { -0.18905f, 0.5475f, -1.0f };

	Vector3 vCursor = { -0.535f, 0.4f, -1.0f };
	Vector3 vCursorW = { -0.535f, 0.33f, -1.0f };
	Vector3 vCursorA = { -0.535f, 0.26f, -1.0f };
	Vector3 vCursorS = { -0.535f, 0.19f, -1.0f };
	Vector3 vCursorD = { -0.535f, 0.12f, -1.0f };
	Vector3 vCursorQ = { -0.535f, 0.05f, -1.0f };
	Vector3 vCursorE = { -0.535f, -0.02f, -1.0f };

	Vector3 vQuad = { 0.1f, 0.4f, -1.0f };
	Vector3 vQuadSpace = { 0.1f, 0.33f, -1.0f };
	Vector3 vQuadRemove = { 0.1f, 0.26f, -1.0f };
	
	Vector3 vQuadColours = { 0.1f, 0.1375f, -1.0f };
	Vector3 vQuadRed = { 0.1f, 0.0675f, -1.0f };
	Vector3 vQuadGreen = { 0.1f, -0.0025f, -1.0f };
	Vector3 vQuadBlue = { 0.1f, -0.0725f, -1.0f };
	Vector3 vQuadCyan = { 0.1f, -0.1425f, -1.0f };
	Vector3 vQuadMagenta = { 0.1f, -0.2125f, -1.0f };
	Vector3 vQuadYellow = { 0.1f, -0.2825f, -1.0f };

	Vector3 vReturn = { -0.2195f, -0.5495f, -1.0f };

	Color cWhite = { 1.0f, 1.0f, 1.0f };
	Color cRed = { 1.0f, 0.2f, 0.2f };
	Color cGreen = { 0.0f, 1.0f, 0.0f };
	Color cBlue = { 0.2f, 0.2f, 1.0f };
	Color cCyan = { 0.0f, 1.0f, 1.0f };
	Color cMagenta = { 1.0f, 0.0f, 1.0f };
	Color cYellow = { 1.0f, 1.0f, 0.0f };
	Color cOrange = { 1.0f, 0.7f, 0.0f };

	DrawString("Game Scene Controls", &vTitle, &cRed);

	DrawString("Cursor", &vCursor, &cYellow);
	DrawString("'w' - Move cursor backwards", &vCursorW, &cWhite);
	DrawString("'a' - Move cursor left", &vCursorA, &cWhite);
	DrawString("'s' - Move cursor forward", &vCursorS, &cWhite);
	DrawString("'d' - Move cursor right", &vCursorD, &cWhite);
	DrawString("'q' - Move cursor up", &vCursorQ, &cWhite);
	DrawString("'e' - Move cursor down", &vCursorE, &cWhite);

	DrawString("Quads", &vQuad, &cOrange);
	DrawString("'space' - Define 1 quad vertex", &vQuadSpace, &cWhite);
	DrawString("'n' - Remove quads from scene", &vQuadRemove, &cWhite);
	
	DrawString("Quad Colours", &vQuadColours, &cMagenta);
	DrawString("'r' - Change quad to red", &vQuadRed, &cWhite);
	DrawString("'g' - Change quad to green", &vQuadGreen, &cWhite);
	DrawString("'b' - Change quad to blue", &vQuadBlue, &cWhite);
	DrawString("'c' - Change quad to cyan", &vQuadCyan, &cWhite);
	DrawString("'m' - Change quad to magenta", &vQuadMagenta, &cWhite);
	DrawString("'y' - Change quad to yellow", &vQuadYellow, &cWhite);

	DrawString("'TAB' to return to the scene.", &vReturn, &cRed);

	glEnable(GL_DEPTH_TEST);
}

void SceneGame::SceneAudio()
{
	if (!paused)
	{
		if (audioPlaying)
			PlaySound(NULL, NULL, 0);

		if (colourAudio)
			PlaySound("Audio/button.wav", GetModuleHandle(NULL), SND_ASYNC);
		
		audioPlaying = colourAudio = false;
	}
	else
	{
		if (!audioPlaying)
			PlaySound("Audio/mario_elevator.wav", GetModuleHandle(NULL), SND_LOOP | SND_ASYNC);
		
		audioPlaying = true;
	}
}

void SceneGame::DrawGrid()
{
	for (int i = 0; i < 40; i++)
	{
		glPushMatrix();

		if (i < 20)
			glTranslatef(0, cubeY, i);

		if (i >= 20)
		{
			glTranslatef(i - 20, cubeY, 0);
			glRotatef(-90, 0, 1, 0);
		}

		glBegin(GL_LINES);
			if (cubeY == 1 || cubeY == -1)
				glColor3f(1, 0.7, 0.3);
			else if (cubeY == 2 || cubeY == -2)
				glColor3f(1, 0.5, 0);
			else if (cubeY == 3 || cubeY == -3)
				glColor3f(1, 0.3, 0);
			else if (cubeY == 4 || cubeY == -4)
				glColor3f(1, 0, 0);
			else
				glColor3f(1, 1, 1);
			glLineWidth(1);
			glVertex3f(0, -0.1, 0);
			glVertex3f(19, -0.1, 0);
		glEnd();

		glPopMatrix();
	}
}

void SceneGame::AddQuadRed()
{
	Q_Red[0].state++;
	if (Q_Red[0].state > 4)
		Q_Red[0].state = 1;
	int st = Q_Red[0].state;

	if (st == 1)
	{
		Q_Red[0].total++;
		cubeN = Q_Red[0].total;

		Q_Red[cubeN].x1 = cubeXRed;
		Q_Red[cubeN].y1 = cubeY;
		Q_Red[cubeN].z1 = cubeZRed;
	}

	if (st == 1 || st == 2)
	{
		Q_Red[cubeN].x2 = cubeXRed;
		Q_Red[cubeN].y2 = cubeY;
		Q_Red[cubeN].z2 = cubeZRed;
	}

	if (st == 1 || st == 2 || st == 3)
	{
		Q_Red[cubeN].x3 = cubeXRed;
		Q_Red[cubeN].y3 = cubeY;
		Q_Red[cubeN].z3 = cubeZRed;
		PlaySound("Audio/menu_click.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (st == 1 || st == 2 || st == 3 || st == 4)
	{
		Q_Red[cubeN].x4 = cubeXRed;
		Q_Red[cubeN].y4 = cubeY;
		Q_Red[cubeN].z4 = cubeZRed;
	}

	if (st == 4)
		PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);
}

void SceneGame::AddQuadBlue()
{
	Q_Blue[0].state++;
	if (Q_Blue[0].state > 4)
		Q_Blue[0].state = 1;
	int st = Q_Blue[0].state;

	if (st == 1)
	{
		Q_Blue[0].total++;
		cubeN = Q_Blue[0].total;

		Q_Blue[cubeN].x1 = cubeXBlue;
		Q_Blue[cubeN].y1 = cubeY;
		Q_Blue[cubeN].z1 = cubeZBlue;
	}

	if (st == 1 || st == 2)
	{
		Q_Blue[cubeN].x2 = cubeXBlue;
		Q_Blue[cubeN].y2 = cubeY;
		Q_Blue[cubeN].z2 = cubeZBlue;
	}

	if (st == 1 || st == 2 || st == 3)
	{
		Q_Blue[cubeN].x3 = cubeXBlue;
		Q_Blue[cubeN].y3 = cubeY;
		Q_Blue[cubeN].z3 = cubeZBlue;
		PlaySound("Audio/menu_click.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (st == 1 || st == 2 || st == 3 || st == 4)
	{
		Q_Blue[cubeN].x4 = cubeXBlue;
		Q_Blue[cubeN].y4 = cubeY;
		Q_Blue[cubeN].z4 = cubeZBlue;
	}

	if (st == 4)
		PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);
}

void SceneGame::DrawQuad()
{
	for (int i = 0; i < Q_Red[0].total + 1; i++)
	{
		glBegin(GL_QUADS);
			glColor3f(Q_Red[i].r, Q_Red[i].g, Q_Red[i].b);
			glVertex3f(Q_Red[i].x1, Q_Red[i].y1, Q_Red[i].z1);
			glVertex3f(Q_Red[i].x2, Q_Red[i].y2, Q_Red[i].z2);
			glVertex3f(Q_Red[i].x3, Q_Red[i].y3, Q_Red[i].z3);
			glVertex3f(Q_Red[i].x4, Q_Red[i].y4, Q_Red[i].z4);
		glEnd();
	}
	for (int i = 0; i < Q_Blue[0].total + 1; i++)
	{
		glBegin(GL_QUADS);
			glColor3f(Q_Blue[i].r, Q_Blue[i].g, Q_Blue[i].b);
			glVertex3f(Q_Blue[i].x1, Q_Blue[i].y1, Q_Blue[i].z1);
			glVertex3f(Q_Blue[i].x2, Q_Blue[i].y2, Q_Blue[i].z2);
			glVertex3f(Q_Blue[i].x3, Q_Blue[i].y3, Q_Blue[i].z3);
			glVertex3f(Q_Blue[i].x4, Q_Blue[i].y4, Q_Blue[i].z4);
		glEnd();
	}
}

void SceneGame::DrawCubes()
{
	glPushMatrix();

		if (!cubeRed)
			glColor3f(0.5, 0, 0);
		else
			glColor3f(1, 0, 0);

		glTranslatef(cubeXRed, cubeY, cubeZRed);
		glutSolidCube(0.4);

	glPopMatrix();

	glPushMatrix();

		if (!cubeBlue)
			glColor3f(0, 0.3, 0.5);
		else
			glColor3f(0, 0.6, 1);

		glTranslatef(cubeXBlue, cubeY, cubeZBlue);
		glutSolidCube(0.4);

	glPopMatrix();
}