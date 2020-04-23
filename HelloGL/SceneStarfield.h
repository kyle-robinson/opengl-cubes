#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "Cube.h"
#include "SceneObject.h"

#define OBJECTCOUNT		500
#define MOVEMENT_SPEED	5.0f
#define CUBE_ROTATION	10.0f

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
	void InitMenu();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);

	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawUI();
	void DrawMenu();

	void SceneAudio();
	void CubeMovement(int iterator);

	void CameraMovement();
	void ResetCamera();

	void MouseMenu(int value);

private:
	bool paused, audioPlaying, colorAudio, textureAudio,
		 colorIsRed, colorIsGreen, colorIsBlue, colorIsCyan, colorIsMagenta, colorIsYellow,
		 zReverse, zMoving, cRotating;
	float rotation;
	int mouseMenu, textureMenu, colourMenu;

	Mesh* cubeMesh;
	Texture2D *texturePenguins, *textureStars;
	SceneObject* objects[OBJECTCOUNT];
};