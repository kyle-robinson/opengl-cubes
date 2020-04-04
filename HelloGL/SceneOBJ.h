#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SceneManager.h"
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

private:
	Camera* camera;
	
	ModelOBJ tankObj, cubeObj, skullObj;
	float g_rotation, xPosition, yPosition, zPosition;
	bool tankLoaded, cubeLoaded, skullLoaded;

	Vector4* _lightPosition;
	Lighting* _lightData;
};