#pragma once

#include <Windows.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "ModelOBJ.h"

class SceneOBJ : Scene
{
public:
	SceneOBJ();
	~SceneOBJ(void);

	void Display();
	void Update();

	void InitGL();
	void InitLighting();
	void InitObjects();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);

	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawUI();
	void DrawMenu();

	void SceneAudio();
	void LoadOBJ();

private:
	bool paused, audioPlaying, objectAudio,
		 tankLoaded, cubeLoaded, skullLoaded;
	float g_rotation, xPosition, yPosition, zPosition;

	ModelOBJ tankObj, cubeObj, skullObj;
};