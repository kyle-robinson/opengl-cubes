#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"

#include "Cube.h"
#include "SceneObject.h"
#include "SceneManager.h"

#define OBJECTCOUNT		1
#define MOVEMENT_SPEED	0.1f

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

	void DrawGrid();
	void AddQuad();
	void DrawQuad();
	void DrawCube();

private:
	int cubeX, cubeY, cubeZ, cubeN;
	bool paused, audioPlaying;

	Camera* camera;
	Quads Q[100];

	Vector4* _lightPosition;
	Lighting* _lightData;
};