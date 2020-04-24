#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"
#include "Cube.h"
#include "SceneObject.h"

#define OBJECTCOUNT			2
#define CUBE_MOVEMENT		0.03
#define COLLISION_AMOUNT	0.05
#define COLLISION_TIMER		5

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
	void InitMenu();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);
	
	void KeyboardUp(unsigned char key, int x, int y);
	void KeyboardSpecialUp(int key, int x, int y);
	
	void DrawString(const char* text, Vector3* position, Color* color);
	void DrawMenu();
	void DrawUI();
	
	void SceneAudio();

	void CubeCollision();
	float CalculateDistanceSquared(SceneObject* c1, SceneObject* c2);
	void CubeScreenCollision(int iterator);

	void MovePenguinCube();
	void MoveStarCube();

	void SetCubePositions();
	void UpdateCubePositions();

	void MouseMenu(int value);

private:
	bool paused, audioPlaying, cubeCollision, cubeAudio, colorAudio,
		 colorIsRed, colorIsGreen, colorIsBlue, colorIsCyan, colorIsMagenta, colorIsYellow,
		 innerCollision, upperCollision, outerCollision, lowerCollision,
		 penguinMovingUp, penguinMovingDown, penguinMovingLeft, penguinMovingRight,
		 starMovingUp, starMovingDown, starMovingLeft, starMovingRight;
	float rotation, cubeSpeed, collisionTimer;
	int subMenus, colourMenu, helpMenu;

	Mesh* cubeMesh;
	Texture2D *texturePenguins, *textureStars;
	SceneObject* objects[OBJECTCOUNT];
};