#include "SceneManager.h"
#include "Scene.h"
#include "SceneStarfield.h"

#include <iostream>

SceneManager::SceneManager(int argc, char* argv[], SCENES startScene)
{
	mCurrentScene = NULL;
	ChangeScene(argc, argv, startScene);
}

SceneManager::~SceneManager(void)
{
	delete mCurrentScene;
	mCurrentScene = NULL;
}

void SceneManager::Display()
{
	mCurrentScene->Display();
}

void SceneManager::Update()
{
	mCurrentScene->Update();
}

void SceneManager::ChangeScene(int argc, char* argv[], SCENES newScreen)
{
	if (mCurrentScene != NULL)
	{
		delete mCurrentScene;
	}

	SceneStarfield* tempStarfield;

	switch (newScreen)
	{
	case SCENE_STARFIELD:
		tempStarfield = new SceneStarfield(argc, argv);
		mCurrentScene = (Scene*)tempStarfield;
		tempStarfield = NULL;
		break;
	default:
		std::cout << "ERROR::Was not able to load any of the scenes!" << std::endl;
	}
}