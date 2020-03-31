#pragma once

#include <Windows.h>
#include "Structures.h"

class Scene;
class SceneManager
{
public:
	SceneManager(int argc, char* argv[], SCENES startScene);
	~SceneManager(void);

	void Display();
	void Update();
	void ChangeScene(int argc, char* argv[], SCENES newScene);

private:
	Scene* mCurrentScene;
};