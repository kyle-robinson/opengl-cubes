#pragma once

#include <Windows.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "Scene.h"

#define CUBE_TIMER		10

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
	void DrawString(const char* text, Vector3* position, Color* color);

	void DrawUI();
	void DrawMenu();

	void SceneAudio();

	void DrawGrid();
	void AddQuad();
	void DrawQuad();
	void DrawCube();

private:
	int cubeX, cubeY, cubeZ, cubeN, cubeTimer;
	bool paused, audioPlaying, cubeRed, colourAudio;

	Quads Q[100];
};