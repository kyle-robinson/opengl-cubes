#include "SceneGame.h"
#include "MeshLoader.h"

#include <iostream>

SceneGame::SceneGame() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	cubeX = cubeY = cubeZ = cubeN = 0;
	paused = audioPlaying = false;

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
}

void SceneGame::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	if (paused)
	{
		glDisable(GL_DEPTH_TEST);
		DrawMenu();
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glTranslatef(-13, 3.5, -46);
		glRotatef(40, 1, 1, 0);

		DrawGrid();
		DrawQuad();
		DrawCube();

		glDisable(GL_DEPTH_TEST);
		DrawUI();
		glEnable(GL_DEPTH_TEST);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
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
		cubeZ -= 1;
	if (key == 's')
		cubeZ += 1;

	if (key == 'a')
		cubeX -= 1;
	if (key == 'd')
		cubeX += 1;

	if (key == 'q')
		cubeY += 1;
	if (key == 'e')
		cubeY -= 1;

	if (key == 32)
		AddQuad();

	if (key == 'r')
	{
		Q[cubeN].r = 1;
		Q[cubeN].g = 0;
		Q[cubeN].b = 0;
	}
	if (key == 'g')
	{
		Q[cubeN].r = 0;
		Q[cubeN].g = 1;
		Q[cubeN].b = 0;
	}
	if (key == 'b')
	{
		Q[cubeN].r = 0;
		Q[cubeN].g = 0;
		Q[cubeN].b = 1;
	}
	if (key == 'c')
	{
		Q[cubeN].r = 0;
		Q[cubeN].g = 1;
		Q[cubeN].b = 1;
	}
	if (key == 'm')
	{
		Q[cubeN].r = 1;
		Q[cubeN].g = 0;
		Q[cubeN].b = 1;
	}
	if (key == 'y')
	{
		Q[cubeN].r = 1;
		Q[cubeN].g = 1;
		Q[cubeN].b = 0;
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
	Vector3 vTitle = { 0.25, 8.575f, -2.0f };
	Vector3 vReturn = { 11.15f, -27.05f, -2.0f };

	Color cWhite = { 1.0f, 1.0f, 1.0f };

	DrawString("OpenGL Game", &vTitle, &cWhite);
	DrawString("'TAB' to view scene controls.", &vReturn, &cWhite);
}

void SceneGame::DrawMenu()
{
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
	Vector3 vQuadRed = { 0.1f, 0.26f, -1.0f };
	Vector3 vQuadGreen = { 0.1f, 0.19f, -1.0f };
	Vector3 vQuadBlue = { 0.1f, 0.12f, -1.0f };
	Vector3 vQuadCyan = { 0.1f, 0.05f, -1.0f };
	Vector3 vQuadMagenta = { 0.1f, -0.02f, -1.0f };
	Vector3 vQuadYellow = { 0.1f, -0.09f, -1.0f };

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
	DrawString("'r' - Change quad to red", &vQuadRed, &cWhite);
	DrawString("'g' - Change quad to green", &vQuadGreen, &cWhite);
	DrawString("'b' - Change quad to blue", &vQuadBlue, &cWhite);
	DrawString("'c' - Change quad to cyan", &vQuadCyan, &cWhite);
	DrawString("'m' - Change quad to magenta", &vQuadMagenta, &cWhite);
	DrawString("'y' - Change quad to yellow", &vQuadYellow, &cWhite);

	DrawString("'TAB' to return to the scene.", &vReturn, &cRed);
}

void SceneGame::DrawGrid()
{
	for (int i = 0; i < 40; i++)
	{
		glPushMatrix();
		if (i < 20)
			glTranslatef(0, 0, i);
		if (i >= 20)
		{
			glTranslatef(i - 20, 0, 0);
			glRotatef(-90, 0, 1, 0);
		}

		glBegin(GL_LINES);
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
	}
	if (st == 1)
	{
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
	}
	if (st == 1 || st == 2 || st == 3 || st == 4)
	{
		Q[cubeN].x4 = cubeX;
		Q[cubeN].y4 = cubeY;
		Q[cubeN].z4 = cubeZ;
	}
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
		glColor3f(1, 1, 1);
		glTranslatef(cubeX, cubeY, cubeZ);
		glutSolidCube(0.4);
	glPopMatrix();
}