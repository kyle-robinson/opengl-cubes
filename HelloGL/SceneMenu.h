#pragma once

#include <Windows.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "SceneManager.h"
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

private:
	Camera* camera;

	Texture2D* textureStarfield;
	Texture2D* textureCollision;
	Texture2D* textureGame;
	Texture2D* textureLoader;

	Vector4* _lightPosition;
	Lighting* _lightData;
};