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
	void InitMenu();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);

	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawUI();
	void DrawMenu();

	void SceneAudio();
	void LoadOBJ();
	void UpdateLighting();
	
	void MouseMenu(int value);

	ModelOBJ tankObj, cubeObj, skullObj;
	Material* _material;

private:
	bool paused, audioPlaying, objectAudio, colorAudio,
		 colorIsRed, colorIsGreen, colorIsBlue, colorIsCyan, colorIsMagenta, colorIsYellow,
		 tankLoaded, cubeLoaded, skullLoaded;
	float g_rotation, xPosition, yPosition, zPosition,
		  red, green, blue;
	int subMenus, objectMenu, colourMenu, helpMenu;
};