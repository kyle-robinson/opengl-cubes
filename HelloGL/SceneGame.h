#pragma once

#include <Windows.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "Scene.h"

class SceneGame : Scene
{
public:
	SceneGame();
	~SceneGame(void);

	void Display();
	void Update();

	void InitGL();
	void InitLighting();
	void InitObjects();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);

	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawUI();
	void DrawMenu();

	void SceneAudio();

	void DrawGrid();
	void DrawCubes();
	
	void AddQuadRed();
	void AddQuadBlue();
	void DrawQuad();

private:
	int cubeN,
		cubeXRed, cubeYRed, cubeZRed,
		cubeXBlue, cubeYBlue, cubeZBlue;

	bool paused, audioPlaying, colourAudio,
		 cubeRed, cubeRed_Quad,
		 cubeBlue, cubeBlue_Quad,
		 quadClear;

	Quads Q_Red[100], Q_Blue[100];
};