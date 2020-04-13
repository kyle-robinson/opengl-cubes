#pragma once

#include <Windows.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "Texture2D.h"

class SceneMenu : Scene
{
public:
	SceneMenu();
	~SceneMenu(void);

	void Display();
	void Update();

	void InitGL();
	void InitLighting();
	void InitObjects();

	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawQuads();
	void DrawUI();

private:
	Texture2D *textureStarfield, *textureCollision, *textureGame, *textureLoader;
};