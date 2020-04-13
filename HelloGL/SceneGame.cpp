#include "SceneGame.h"

SceneGame::SceneGame() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	cubeTimer = CUBE_TIMER;

	glutMainLoop();
}

SceneGame::~SceneGame(void)
{

}

void SceneGame::InitGL()
{
	GLUTCallbacks::Init(this);
	glutDisplayFunc(GLUTCallbacks::Display);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, 1.0f, 0.1f, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void SceneGame::InitLighting()
{
	Scene::InitLighting();
}

void SceneGame::InitObjects()
{
	Scene::InitObjects();
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
		DrawCube();

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
	if (key == 'w' && cubeZ > 0)
		cubeZ -= 1;

	if (key == 's' && cubeZ < 19)
		cubeZ += 1;

	if (key == 'a' && cubeX > 0)
		cubeX -= 1;

	if (key == 'd' && cubeX < 19)
		cubeX += 1;

	if (key == 'q' && cubeY < 4)
		cubeY += 1;

	if (key == 'e' && cubeY > -4)
		cubeY -= 1;

	if (key == 'n')
	{
		for (int i = 0; i < Q[0].total + 1; i++)
		{
			Q[i].x1 = 0;
			Q[i].x2 = 0;
			Q[i].x3 = 0;
			Q[i].x4 = 0;
			
			Q[i].y1 = 0;
			Q[i].y2 = 0;
			Q[i].y3 = 0;
			Q[i].y4 = 0;
			
			Q[i].z1 = 0;
			Q[i].z2 = 0;
			Q[i].z3 = 0;
			Q[i].z4 = 0;
		}
		PlaySound("Audio/hint.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (key == 32)
	{
		AddQuad();
		cubeRed = true;
	}

	if (key == 'r')
	{
		Q[cubeN].r = 1;
		Q[cubeN].g = 0;
		Q[cubeN].b = 0;
		colourAudio = true;
	}

	if (key == 'g')
	{
		Q[cubeN].r = 0;
		Q[cubeN].g = 1;
		Q[cubeN].b = 0;
		colourAudio = true;
	}

	if (key == 'b')
	{
		Q[cubeN].r = 0;
		Q[cubeN].g = 0;
		Q[cubeN].b = 1;
		colourAudio = true;
	}

	if (key == 'c')
	{
		Q[cubeN].r = 0;
		Q[cubeN].g = 1;
		Q[cubeN].b = 1;
		colourAudio = true;
	}

	if (key == 'm')
	{
		Q[cubeN].r = 1;
		Q[cubeN].g = 0;
		Q[cubeN].b = 1;
		colourAudio = true;
	}

	if (key == 'y')
	{
		Q[cubeN].r = 1;
		Q[cubeN].g = 1;
		Q[cubeN].b = 0;
		colourAudio = true;
	}

	if (key == 9)
	{
		if (!paused)
			paused = true;
		else
			paused = false;
	}
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
	Vector3 vQuadRed = { 0.1f, 0.19f, -1.0f };
	Vector3 vQuadGreen = { 0.1f, 0.12f, -1.0f };
	Vector3 vQuadBlue = { 0.1f, 0.05f, -1.0f };
	Vector3 vQuadCyan = { 0.1f, -0.02f, -1.0f };
	Vector3 vQuadMagenta = { 0.1f, -0.09f, -1.0f };
	Vector3 vQuadYellow = { 0.1f, -0.16f, -1.0f };

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

		if (cubeRed)
		{
			cubeTimer -= 1;
			if (cubeTimer <= 0)
			{
				cubeRed = false;
				cubeTimer = CUBE_TIMER;
			}
		}

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

void SceneGame::AddQuad()
{
	Q[0].state++;
	if (Q[0].state > 4)
		Q[0].state = 1;
	int st = Q[0].state;

	if (st == 1)
	{
		Q[0].total++;
		cubeN = Q[0].total;

		Q[cubeN].x1 = cubeX;
		Q[cubeN].y1 = cubeY;
		Q[cubeN].z1 = cubeZ;
	}

	if (st == 1 || st == 2)
	{
		Q[cubeN].x2 = cubeX;
		Q[cubeN].y2 = cubeY;
		Q[cubeN].z2 = cubeZ;
	}

	if (st == 1 || st == 2 || st == 3)
	{
		Q[cubeN].x3 = cubeX;
		Q[cubeN].y3 = cubeY;
		Q[cubeN].z3 = cubeZ;
		PlaySound("Audio/menu_click.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (st == 1 || st == 2 || st == 3 || st == 4)
	{
		Q[cubeN].x4 = cubeX;
		Q[cubeN].y4 = cubeY;
		Q[cubeN].z4 = cubeZ;
	}

	if (st == 4)
		PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);
}

void SceneGame::DrawQuad()
{
	for (int i = 0; i < Q[0].total + 1; i++)
	{
		glBegin(GL_QUADS);
			glColor3f(Q[i].r, Q[i].g, Q[i].b);
			glVertex3f(Q[i].x1, Q[i].y1, Q[i].z1);
			glVertex3f(Q[i].x2, Q[i].y2, Q[i].z2);
			glVertex3f(Q[i].x3, Q[i].y3, Q[i].z3);
			glVertex3f(Q[i].x4, Q[i].y4, Q[i].z4);
		glEnd();
	}
}

void SceneGame::DrawCube()
{
	glPushMatrix();

		if (!cubeRed)
			glColor3f(1, 1, 1);
		else
			glColor3f(1, 0, 0);

		glTranslatef(cubeX, cubeY, cubeZ);
		glutSolidCube(0.4);

	glPopMatrix();
}