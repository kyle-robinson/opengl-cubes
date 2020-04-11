#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"

#include "Cube.h"
#include "Pyramid.h"
#include "SceneObject.h"

#include "SceneManager.h"

#define OBJECTCOUNT		500
#define MOVEMENT_SPEED	5.0f

class SceneStarfield : Scene
{
public:
	SceneStarfield();
	~SceneStarfield(void);
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

private:
	bool paused, audioPlaying, colorAudio, textureAudio;
	float rotation;

	Mesh* cubeMesh;
	Mesh* pyramidMesh;
	Texture2D* texturePenguins;
	Texture2D* textureStars;

	Camera* camera;
	SceneObject* objects[OBJECTCOUNT];

	Vector4 * _lightPosition;
	Lighting* _lightData;

	bool colorIsRed, colorIsGreen, colorIsBlue, colorIsCyan, colorIsMagenta, colorIsYellow;
	bool zReverse, zMoving, cRotating;
};