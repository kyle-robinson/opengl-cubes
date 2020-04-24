#include "SceneOBJ.h"

SceneOBJ::SceneOBJ()
{
	InitGL();
	InitLighting();
	InitObjects();
	InitMenu();

	tankLoaded = true;
	xPosition = 0.0f;
	yPosition = -1.0f;
	zPosition = -5.0f;

	glutMainLoop();
}

SceneOBJ::~SceneOBJ(void)
{	
	Scene::~Scene();

	tankObj.Release();
	skullObj.Release();
	cubeObj.Release();

	delete _material;
	_material = NULL;
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
	_material = new Material();
	red = green = blue = 1.0f;
	UpdateLighting();
}

void SceneOBJ::InitObjects()
{
	Scene::InitObjects();
	tankObj.Load((char*)"Objects/IS7.obj");
	cubeObj.Load((char*)"Objects/cube.obj");
	skullObj.Load((char*)"Objects/skull.obj");
}

void SceneOBJ::InitMenu()
{
	objectMenu = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddMenuEntry("Tank", 0);
	glutAddMenuEntry("Skull", 1);
	glutAddMenuEntry("Cube", 2);

	colourMenu = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddMenuEntry("Red", 3);
	glutAddMenuEntry("Green", 4);
	glutAddMenuEntry("Blue", 5);
	glutAddMenuEntry("Cyan", 6);
	glutAddMenuEntry("Magenta", 7);
	glutAddMenuEntry("Yellow", 8);

	helpMenu = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddMenuEntry("Controls", 9);
	glutAddMenuEntry("Exit", 10);

	subMenus = glutCreateMenu(GLUTCallbacks::MouseMenu);
	glutAddSubMenu("Object", objectMenu);
	glutAddSubMenu("Colour", colourMenu);
	glutAddSubMenu("Help", helpMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void SceneOBJ::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateLighting();

	if (!paused)
	{
		glDisable(GL_TEXTURE_2D);
		LoadOBJ();
		DrawUI();
		glEnable(GL_TEXTURE_2D);
	}
	else
		DrawMenu();

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
	switch (key)
	{
	case 'w':
	case 'W':
		if (tankLoaded)
			zPosition -= 0.2f;
		else if (cubeLoaded)
			zPosition -= 0.1f;
		else if (skullLoaded)
			zPosition -= 2.0f;
		break;

	case 'a':
	case 'A':
		if (tankLoaded)
			xPosition -= 0.2f;
		else if (cubeLoaded)
			xPosition -= 0.1f;
		else if (skullLoaded)
			xPosition -= 2.0f;
		break;

	case 's':
	case 'S':
		if (tankLoaded)
			zPosition += 0.2f;
		else if (cubeLoaded)
			zPosition += 0.1f;
		else if (skullLoaded)
			zPosition += 2.0f;
		break;

	case 'd':
	case 'D':
		if (tankLoaded)
			xPosition += 0.2f;
		else if (cubeLoaded)
			xPosition += 0.1f;
		else if (skullLoaded)
			xPosition += 2.0f;
		break;

	case 't':
	case 'T':
		xPosition = 0.0f;
		yPosition = -1.0f;
		zPosition = -5.0f;
		tankLoaded = objectAudio = true;
		cubeLoaded = skullLoaded = false;
		break;

	case 'h':
	case 'H':
		xPosition = 0.0f;
		yPosition = -10.0f;
		zPosition = -30.0f;
		skullLoaded = objectAudio = true;
		tankLoaded = cubeLoaded = false;
		break;

	case 'c':
	case 'C':
		xPosition = 0.0f;
		yPosition = -0.25f;
		zPosition = -0.5f;
		cubeLoaded = objectAudio = true;
		tankLoaded = skullLoaded = false;
		break;

	case 'r':
	case 'R':
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
		colorAudio = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
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
}

void SceneOBJ::KeyboardSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (tankLoaded)
			yPosition += 0.2f;
		else if (cubeLoaded)
			yPosition += 0.1f;
		else if (skullLoaded)
			yPosition += 2.0f;
		break;

	case GLUT_KEY_DOWN:
		if (tankLoaded)
			yPosition -= 0.2f;
		else if (cubeLoaded)
			yPosition -= 0.1f;
		else if (skullLoaded)
			yPosition -= 2.0f;
		break;

	case GLUT_KEY_F1:
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
		colorAudio = colorIsRed = true;
		colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
		break;

	case GLUT_KEY_F2:
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
		colorAudio = colorIsGreen = true;
		colorIsRed = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
		break;

	case GLUT_KEY_F3:
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
		colorAudio = colorIsBlue = true;
		colorIsRed = colorIsGreen = colorIsCyan = colorIsMagenta = colorIsYellow = false;
		break;

	case GLUT_KEY_F4:
		red = 0.0f;
		green = 1.0f;
		blue = 1.0f;
		colorAudio = colorIsCyan = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsMagenta = colorIsYellow = false;
		break;

	case GLUT_KEY_F5:
		red = 1.0f;
		green = 0.0f;
		blue = 1.0f;
		colorAudio = colorIsMagenta = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsYellow = false;
		break;

	case GLUT_KEY_F6:
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
		colorAudio = colorIsYellow = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = false;
		break;

	default:
		break;
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

	Vector3 vTitle =		{ -1.8f, 1.7f, -1.0f };
	Vector3 vReturn =		{ -1.05f, -1.75f, -1.0f };

	Color cWhite =			{ 1.0f, 1.0f, 1.0f };
	Color cPurple =			{ 0.6f, 0.2f, 0.9f };
	Color cLightPurple =	{ 0.8f, 0.5f, 1.0f };
	Color cLilac =			{ 0.9f, 0.8f, 1.0f };

	DrawString("O B J    L O A D E R", &vTitle, &cPurple);
	DrawString("`T A B'    t o    v i e w    s c e n e    c o n t r o l s    . . .", &vReturn, &cLilac);

	Vector3 vObjLoaded = { 0.5f, 1.7f, -1.0f };
	if (tankLoaded)
		DrawString("IS7 Tank Object Loaded", &vObjLoaded, &cWhite);
	else if (skullLoaded)
		DrawString("Human Skull Object Loaded", &vObjLoaded, &cWhite);
	else if (cubeLoaded)
		DrawString("Companion Cube Object Loaded", &vObjLoaded, &cWhite);

	glEnable(GL_LIGHTING);
	
	Vector3 vPosition = { 0.5f, 1.5f, -1.0f };
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
}

void SceneOBJ::DrawMenu()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	Vector3 vTitle =		 { -0.85f, 1.75f, -1.0f };

	Vector3 vMovement =		 { -1.7f, 1.25f, -1.0f };
	Vector3 vMovementW =	 { -1.7f, 1.05f, -1.0f };
	Vector3 vMovementA =	 { -1.7f, 0.85f, -1.0f };
	Vector3 vMovementS =	 { -1.7f, 0.65f, -1.0f };
	Vector3 vMovementD =	 { -1.7f, 0.45f, -1.0f };
	Vector3 vMovementUp =	 { -1.7f, 0.25f, -1.0f };
	Vector3 vMovementDown =  { -1.7f, 0.05f, -1.0f };

	Vector3 vObject =		 { 0.25f, 1.25f, -1.0f };
	Vector3 vObjectTank =	 { 0.25f, 1.05f, -1.0f };
	Vector3 vObjectSkull =	 { 0.25f, 0.85f, -1.0f };
	Vector3 vObjectCube =	 { 0.25f, 0.65f, -1.0f };

	Vector3 vColour =		 { 0.25f, 0.3f, -1.0f };
	Vector3 vColourRed =	 { 0.25f, 0.1f, -1.0f };
	Vector3 vColourGreen =	 { 0.25f, -0.1f, -1.0f };
	Vector3 vColourBlue =	 { 0.25f, -0.3f, -1.0f };
	Vector3 vColourCyan =	 { 0.25f, -0.5f, -1.0f };
	Vector3 vColourMagenta = { 0.25f, -0.7f, -1.0f };
	Vector3 vColourYellow =  { 0.25f, -0.9f, -1.0f };
	Vector3 vColourReset =	 { 0.25f, -1.1f, -1.0f };

	Vector3 vReturn =		 { -1.05f, -1.75f, -1.0f };

	Color cWhite =			 { 1.0f, 1.0f, 1.0f };
	Color cRed =			 { 1.0f, 0.2f, 0.2f };
	Color cGreen =			 { 0.0f, 1.0f, 0.0f };
	Color cBlue =			 { 0.2f, 0.2f, 1.0f };
	Color cCyan =			 { 0.0f, 1.0f, 1.0f };
	Color cMagenta =		 { 1.0f, 0.0f, 1.0f };
	Color cYellow =			 { 1.0f, 1.0f, 0.0f };
	Color cOrange =			 { 1.0f, 0.7f, 0.0f };
	Color cPurple =			 { 0.6f, 0.2f, 0.9f };
	Color cLightPurple =	 { 0.8f, 0.5f, 1.0f };
	Color cLilac =			 { 0.9f, 0.8f, 1.0f };

	DrawString("O B J    L O A D E R    C O N T R O L S", &vTitle, &cPurple);

	DrawString("o b j e c t    m o v e m e n t", &vMovement, &cLightPurple);
	DrawString("`W' - Move Backward", &vMovementW, &cWhite);
	DrawString("`A' - Move Left", &vMovementA, &cWhite);
	DrawString("`S' - Move Forward", &vMovementS, &cWhite);
	DrawString("`D' - Move Right", &vMovementD, &cWhite);
	DrawString("`UP' - Move Up", &vMovementUp, &cWhite);
	DrawString("`DOWN' - Move Down", &vMovementDown, &cWhite);

	DrawString("c h a n g e    o b j e c t", &vObject, &cLightPurple);
	DrawString("`T' - Tank", &vObjectTank, &cWhite);
	DrawString("`H' - Skull", &vObjectSkull, &cWhite);
	DrawString("`C' - Cube", &vObjectCube, &cWhite);

	DrawString("o b j e c t    c o l o u r", &vColour, &cLightPurple);
	DrawString("`F1' - Red", &vColourRed, &cWhite);
	DrawString("`F2' - Green", &vColourGreen, &cWhite);
	DrawString("`F3' - Blue", &vColourBlue, &cWhite);
	DrawString("`F4' - Cyan", &vColourCyan, &cWhite);
	DrawString("`F5' - Magenta", &vColourMagenta, &cWhite);
	DrawString("`F6' - Yellow", &vColourYellow, &cWhite);
	DrawString("`R' - Reset Colours", &vColourReset, &cWhite);

	DrawString("`T A B'    t o    r e t u r n    t o    t h e    s c e n e    . . .", &vReturn, &cLilac);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
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
			PlaySound("Audio/pipe.wav", GetModuleHandle(NULL), SND_ASYNC);
		if (colorAudio)
			PlaySound("Audio/button.wav", GetModuleHandle(NULL), SND_ASYNC);
		audioPlaying = objectAudio = colorAudio = false;
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
	else if (skullLoaded)
	{
		glTranslatef(xPosition, yPosition, zPosition);
		glRotatef(g_rotation, 0, 1, 0);
		glRotatef(90, 0, 1, 0);
		g_rotation++;
		skullObj.Draw();
	}
	else if (cubeLoaded)
	{
		glTranslatef(xPosition, yPosition, zPosition);
		glRotatef(g_rotation, 0, 1, 0);
		glRotatef(90, 0, 1, 0);
		g_rotation++;
		cubeObj.Draw();
	}
	glPopMatrix();
}

void SceneOBJ::UpdateLighting()
{
	_material->Ambient.x = red;
	_material->Ambient.y = green;
	_material->Ambient.z = blue;
	_material->Ambient.w = 1.0;

	_material->Diffuse.x = red;
	_material->Diffuse.y = green;
	_material->Diffuse.z = blue;
	_material->Diffuse.w = 1.0;

	_material->Specular.x = 1.0;
	_material->Specular.y = 1.0;
	_material->Specular.z = 1.0;
	_material->Specular.w = 1.0;

	_material->Shininess = 100.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->Ambient.x));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->Diffuse.x));
	glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->Specular.x));
	glMaterialf(GL_FRONT, GL_SHININESS, _material->Shininess);
}

void SceneOBJ::MouseMenu(int value)
{
	switch (value)
	{
	case 0:
		xPosition = 0.0f;
		yPosition = -1.0f;
		zPosition = -5.0f;
		tankLoaded = objectAudio = true;
		skullLoaded = cubeLoaded = false;
		break;

	case 1:
		xPosition = 0.0f;
		yPosition = -10.0f;
		zPosition = -30.0f;
		skullLoaded = objectAudio = true;
		tankLoaded = cubeLoaded = false;
		break;

	case 2:
		xPosition = 0.0f;
		yPosition = -0.25f;
		zPosition = -0.5f;
		cubeLoaded = objectAudio = true;
		tankLoaded = skullLoaded = false;
		break;

	case 3:
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
		colorAudio = colorIsRed = true;
		colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = colorIsYellow = false;
		break;

	case 4:
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
		colorAudio = colorIsBlue = true;
		colorIsRed = colorIsGreen = colorIsCyan = colorIsMagenta = colorIsYellow = false;
		break;

	case 5:
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
		colorAudio = colorIsBlue = true;
		colorIsRed = colorIsGreen = colorIsCyan = colorIsMagenta = colorIsYellow = false;
		break;

	case 6:
		red = 0.0f;
		green = 1.0f;
		blue = 1.0f;
		colorAudio = colorIsCyan = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsMagenta = colorIsYellow = false;
		break;

	case 7:
		red = 1.0f;
		green = 0.0f;
		blue = 1.0f;
		colorAudio = colorIsMagenta = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsYellow = false;
		break;

	case 8:
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
		colorAudio = colorIsYellow = true;
		colorIsRed = colorIsGreen = colorIsBlue = colorIsCyan = colorIsMagenta = false;
		break;

	case 9:
		if (!paused)
			paused = true;
		else
			paused = false;
		break;

	case 10:
		glutDestroyWindow(glutGetWindow());
		exit(0);
		break;

	default:
		break;
	}
}