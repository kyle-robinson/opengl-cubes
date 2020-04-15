#include "SceneGame.h"
#include <iostream>
#include <string>
#include <iomanip>

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

	if (key == 'q' && cubeYRed < 4)
		cubeYRed += 1;

	if (key == 'e' && cubeYRed > -4)
		cubeYRed -= 1;

	if (key == 'r')
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
		cubeRed_Quad = false;
		cubeBlue_Quad = false;
		quadClear = true;
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

	if (key == GLUT_KEY_PAGE_UP && cubeYBlue < 4)
		cubeYBlue += 1;

	if (key == GLUT_KEY_PAGE_DOWN && cubeYBlue > -4)
		cubeYBlue -= 1;

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
	Vector3 vRedYPos = { 18.25, 8.575f, -2.0f };
	Vector3 vBlueYPos = { 25.25, 8.575f, -2.0f };
	Vector3 vLogText = { 18.45, 7.075f, -2.0f };
	Vector3 vReturn = { 7.25f, -25.75f, -2.0f };

	Color cWhite = { 1.0f, 1.0f, 1.0f };
	Color cRed = { 1.0f, 0.2f, 0.2f };
	Color cBlue = { 0.2f, 0.2f, 1.0f };
	Color cPalePink = { 1.0f, 0.8f, 1.0f };
	Color cDarkPink = { 0.9f, 0.1f, 0.8f };

	DrawString("O P E N G L    G A M E", &vTitle, &cDarkPink);
	DrawString("`T A B'    t o    v i e w    s c e n e    c o n t r o l s    . . .", &vReturn, &cPalePink);

	std::string strRed = std::to_string(cubeYRed);
	std::string strBlue = std::to_string(cubeYBlue);

	std::string strRedYPos = "Red Y:   " + strRed;
	std::string strBlueYPos = "Blue Y:   " + strBlue;;

	char const* redChar = strRedYPos.c_str();
	char const* blueChar = strBlueYPos.c_str();

	DrawString(redChar, &vRedYPos, &cWhite);
	DrawString(blueChar, &vBlueYPos, &cWhite);

	if (cubeRed_Quad)
		DrawString("Red Cursor Created a Quad", &vLogText, &cRed);
	else if (cubeBlue_Quad)
		DrawString("Blue Cursor Created a Quad", &vLogText, &cBlue);
	else if (quadClear)
		DrawString("All Quads Have Been Removed", &vLogText, &cWhite);

	glEnable(GL_DEPTH_TEST);
}

void SceneGame::DrawMenu()
{
	glDisable(GL_DEPTH_TEST);

	Vector3 vTitle =		{ -0.3f, 0.5475f, -1.0f };

	Vector3 vRedCursor =	{ -0.5f, 0.425f, -1.0f };
	Vector3 vRedW =			{ -0.5f, 0.375f, -1.0f };
	Vector3 vRedA =			{ -0.5f, 0.325f, -1.0f };
	Vector3 vRedS =			{ -0.5f, 0.275f, -1.0f };
	Vector3 vRedD =			{ -0.5f, 0.225f, -1.0f };
	Vector3 vRedQ =			{ -0.5f, 0.175f, -1.0f };
	Vector3 vRedE =			{ -0.5f, 0.125f, -1.0f };
	Vector3 vRedSpace =		{ -0.5f, 0.075f, -1.0f };

	Vector3 vRedF1 =		{ -0.5f, -0.025f, -1.0f };
	Vector3 vRedF2 =		{ -0.5f, -0.075f, -1.0f };
	Vector3 vRedF3 =		{ -0.5f, -0.125f, -1.0f };
	Vector3 vRedF4 =		{ -0.5f, -0.175f, -1.0f };
	Vector3 vRedF5 =		{ -0.5f, -0.225f, -1.0f };
	Vector3 vRedF6 =		{ -0.5f, -0.275f, -1.0f };

	Vector3 vBlueCursor =	{ 0.1f, 0.425f, -1.0f };
	Vector3 vBlueUp =		{ 0.1f, 0.375f, -1.0f };
	Vector3 vBlueLeft =		{ 0.1f, 0.325f, -1.0f };
	Vector3 vBlueDown =		{ 0.1f, 0.275f, -1.0f };
	Vector3 vBlueRight =	{ 0.1f, 0.225f, -1.0f };
	Vector3 vBluePgUp =		{ 0.1f, 0.175f, -1.0f };
	Vector3 vBluePgDown =	{ 0.1f, 0.125f, -1.0f };
	Vector3 vBlueEnter =	{ 0.1f, 0.075f, -1.0f };

	Vector3 vBlueF7 =		{ 0.1f, -0.025f, -1.0f };
	Vector3 vBlueF8 =		{ 0.1f, -0.075f, -1.0f };
	Vector3 vBlueF9 =		{ 0.1f, -0.125f, -1.0f };
	Vector3 vBlueF10 =		{ 0.1f, -0.175f, -1.0f };
	Vector3 vBlueF11 =		{ 0.1f, -0.225f, -1.0f };
	Vector3 vBlueF12 =		{ 0.1f, -0.275f, -1.0f };

	Vector3 vMisc =			{ -0.5f, -0.375f, -1.0f };
	Vector3 vMiscClear =	{ -0.5f, -0.425f, -1.0f };

	Vector3 vReturn =		{ -0.3325f, -0.55175f, -1.0f };

	Color cWhite =		{ 1.0f, 1.0f, 1.0f };
	Color cDarkPink =	{ 0.9f, 0.1f, 0.8f };
	Color cPink =		{ 0.9f, 0.5f, 0.9f };
	Color cPalePink =	{ 1.0f, 0.8f, 1.0f };

	DrawString("G A M E    S C E N E    C O N T R O L S", &vTitle, &cDarkPink);

	DrawString("r e d    c u r s o r", &vRedCursor, &cPink);
	DrawString("`W' - Move Backwards", &vRedW, &cWhite);
	DrawString("`A' - Move Left", &vRedA, &cWhite);
	DrawString("`S' - Move Forward", &vRedS, &cWhite);
	DrawString("`D' - Move Right", &vRedD, &cWhite);
	DrawString("`Q' - Move Up", &vRedQ, &cWhite);
	DrawString("`E' - Move Down", &vRedE, &cWhite);
	DrawString("`SPACE' - Define Quad Vertex", &vRedSpace, &cWhite);

	DrawString("`F1' - Quad to Red", &vRedF1, &cWhite);
	DrawString("`F2' - Quad to Green", &vRedF2, &cWhite);
	DrawString("`F3' - Quad to Blue", &vRedF3, &cWhite);
	DrawString("`F4' - Quad to Cyan", &vRedF4, &cWhite);
	DrawString("`F5' - Quad to Magenta", &vRedF5, &cWhite);
	DrawString("`F6' - Quad to Yellow", &vRedF6, &cWhite);

	DrawString("b l u e    c u r s o r", &vBlueCursor, &cPink);
	DrawString("`UP' - Move Backwards", &vBlueUp, &cWhite);
	DrawString("`LEFT' - Move Left", &vBlueLeft, &cWhite);
	DrawString("`DOWN' - Move Forward", &vBlueDown, &cWhite);
	DrawString("`RIGHT' - Move Right", &vBlueRight, &cWhite);
	DrawString("`PG UP' - Move Up", &vBluePgUp, &cWhite);
	DrawString("`PG DOWN' - Move Down", &vBluePgDown, &cWhite);
	DrawString("`ENTER' - Define Quad Vertex", &vBlueEnter, &cWhite);

	DrawString("`F7' - Quad to Red", &vBlueF7, &cWhite);
	DrawString("`F8' - Quad to Green", &vBlueF8, &cWhite);
	DrawString("`F9' - Quad to Blue", &vBlueF9, &cWhite);
	DrawString("`F10' - Quad to Cyan", &vBlueF10, &cWhite);
	DrawString("`F11' - Quad to Magenta", &vBlueF11, &cWhite);
	DrawString("`F12' - Quad to Yellow", &vBlueF12, &cWhite);

	DrawString("m i s c e l l a n e o u s", &vMisc, &cPink);
	DrawString("`R' - Remove All Quads", &vMiscClear, &cWhite);

	DrawString("`T A B'    t o    r e t u r n    t o    t h e    s c e n e    . . .", &vReturn, &cPalePink);

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
		Q_Red[cubeN].y1 = cubeYRed;
		Q_Red[cubeN].z1 = cubeZRed;
	}

	if (st == 1 || st == 2)
	{
		Q_Red[cubeN].x2 = cubeXRed;
		Q_Red[cubeN].y2 = cubeYRed;
		Q_Red[cubeN].z2 = cubeZRed;
	}

	if (st == 1 || st == 2 || st == 3)
	{
		Q_Red[cubeN].x3 = cubeXRed;
		Q_Red[cubeN].y3 = cubeYRed;
		Q_Red[cubeN].z3 = cubeZRed;
		PlaySound("Audio/menu_click.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (st == 1 || st == 2 || st == 3 || st == 4)
	{
		Q_Red[cubeN].x4 = cubeXRed;
		Q_Red[cubeN].y4 = cubeYRed;
		Q_Red[cubeN].z4 = cubeZRed;
	}

	if (st == 4)
	{
		PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);
		cubeRed_Quad = true;
		cubeBlue_Quad = false;
		quadClear = false;
	}
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
		Q_Blue[cubeN].y1 = cubeYBlue;
		Q_Blue[cubeN].z1 = cubeZBlue;
	}

	if (st == 1 || st == 2)
	{
		Q_Blue[cubeN].x2 = cubeXBlue;
		Q_Blue[cubeN].y2 = cubeYBlue;
		Q_Blue[cubeN].z2 = cubeZBlue;
	}

	if (st == 1 || st == 2 || st == 3)
	{
		Q_Blue[cubeN].x3 = cubeXBlue;
		Q_Blue[cubeN].y3 = cubeYBlue;
		Q_Blue[cubeN].z3 = cubeZBlue;
		PlaySound("Audio/menu_click.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	if (st == 1 || st == 2 || st == 3 || st == 4)
	{
		Q_Blue[cubeN].x4 = cubeXBlue;
		Q_Blue[cubeN].y4 = cubeYBlue;
		Q_Blue[cubeN].z4 = cubeZBlue;
	}

	if (st == 4)
	{
		PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);
		cubeBlue_Quad = true;
		cubeRed_Quad = false;
		quadClear = false;
	}
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

		glTranslatef(cubeXRed, cubeYRed, cubeZRed);
		glutSolidCube(0.4);

	glPopMatrix();

	glPushMatrix();

		if (!cubeBlue)
			glColor3f(0, 0.3, 0.5);
		else
			glColor3f(0, 0.6, 1);

		glTranslatef(cubeXBlue, cubeYBlue, cubeZBlue);
		glutSolidCube(0.4);

	glPopMatrix();
}