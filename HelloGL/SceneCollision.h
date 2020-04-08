#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"

#include "Cube.h"
#include "SceneObject.h"
#include "SceneManager.h"

#define OBJECTCOUNT		2
#define MOVEMENT_SPEED	0.1f

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
	float CalculateDistanceSquared(SceneObject* c1, SceneObject* c2);

private:
	bool paused, audioPlaying, cubeCollision, cubeAudio, colorAudio;
	float rotation;

	Mesh* cubeMesh;
	Texture2D* texturePenguins;
	Texture2D* textureStars;

	Camera* camera;
	SceneObject* objects[OBJECTCOUNT];

	Vector4* _lightPosition;
	Lighting* _lightData;
};