#include "GLUTCallbacks.h"

#include "Scene.h"
#include "SceneManager.h"
#include "SceneMenu.h"

#include "SceneStarfield.h"
#include "SceneCollision.h"
#include "SceneGame.h"
#include "SceneOBJ.h"

#include <iostream>

SceneManager::SceneManager()
{
	mCurrentScene = NULL;

	GLUTCallbacks::Init(this);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);

	ChangeScene(SCENE_MENU);
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

void SceneManager::ChangeScene(SCENES newScreen)
{	
	PlaySound("Audio/game_sound.wav", GetModuleHandle(NULL), SND_ASYNC);
	
	if (mCurrentScene != NULL || mCurrentScene == NULL)
	{
		delete mCurrentScene;
	}

	SceneMenu* tempMenu;
	SceneStarfield* tempStarfield;
	SceneCollision* tempCollision;
	SceneGame* tempGame;
	SceneOBJ* tempOBJ;

	switch (newScreen)
	{
	case SCENE_MENU:
		tempMenu = new SceneMenu();
		mCurrentScene = (Scene*)tempMenu;
		tempMenu = NULL;
		break;
	case SCENE_STARFIELD:
		tempStarfield = new SceneStarfield();
		mCurrentScene = (Scene*)tempStarfield;
		tempStarfield = NULL;
		break;
	case SCENE_COLLISION:
		tempCollision = new SceneCollision();
		mCurrentScene = (Scene*)tempCollision;
		tempCollision = NULL;
		break;
	case SCENE_GAME:
		tempGame = new SceneGame();
		mCurrentScene = (Scene*)tempGame;
		tempGame = NULL;
	case SCENE_OBJLOADER:
		tempOBJ = new SceneOBJ();
		mCurrentScene = (Scene*)tempOBJ;
		tempOBJ = NULL;
	default:
		std::cout << "ERROR::Was not able to load any of the scenes!" << std::endl;
		break;
	}
}

void SceneManager::Keyboard(unsigned char key, int x, int y)
{
	if (key == '0')
	{
		delete mCurrentScene;
		mCurrentScene = NULL;
		ChangeScene(SCENE_MENU);
	}
	if (key == '1')
	{
		delete mCurrentScene;
		mCurrentScene = NULL;
		ChangeScene(SCENE_STARFIELD);
	}
	if (key == '2')
	{
		delete mCurrentScene;
		mCurrentScene = NULL;
		ChangeScene(SCENE_COLLISION);
	}
	if (key == '3')
	{
		delete mCurrentScene;
		mCurrentScene = NULL;
		ChangeScene(SCENE_GAME);
	}
	if (key == '4')
	{
		delete mCurrentScene;
		mCurrentScene = NULL;
		ChangeScene(SCENE_OBJLOADER);
	}

	if (key == 27)
		exit(0);
}