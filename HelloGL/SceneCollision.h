#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "Cube.h"
#include "SceneObject.h"

#define OBJECTCOUNT		2

class SceneCollision : Scene
{
public:
	SceneCollision();
	~SceneCollision(void);

	void Display();
	void Update();

	void InitGL();
	void InitLighting();
	void InitObjects();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);
	
	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawMenu();
	void DrawUI();
	
	void SceneAudio();

	void CubeCollision();
	float CalculateDistanceSquared(SceneObject* c1, SceneObject* c2);
	void CubeScreenCollision(int iterator);

private:
	bool paused, audioPlaying, cubeCollision, cubeAudio, colorAudio,
		 colorIsRed, colorIsGreen, colorIsBlue, colorIsCyan, colorIsMagenta, colorIsYellow;
	float rotation;

	Mesh* cubeMesh;
	Texture2D *texturePenguins, *textureStars;
	SceneObject* objects[OBJECTCOUNT];
};