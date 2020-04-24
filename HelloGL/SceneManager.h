#pragma once

#include <Windows.h>
#include "Structures.h"

#define REFRESHRATE		16

class Scene;
class SceneManager
{
public:
	SceneManager();
	~SceneManager(void);

	void InitGL();

	void Display();
	void Update();

	void ChangeScene(SCENES newScene);
	void Keyboard(unsigned char key, int x, int y);

private:
	Scene* mCurrentScene;
};