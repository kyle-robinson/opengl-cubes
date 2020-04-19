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
	mCurrentScene->~Scene();
}

void SceneManager::Keyboard(unsigned char key, int x, int y)
{
	if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == 27)
	{
		delete mCurrentScene;
		mCurrentScene = NULL;
	}

	if (key == '0')
		ChangeScene(SCENE_MENU);
	else if (key == '1')
		ChangeScene(SCENE_STARFIELD);
	else if (key == '2')
		ChangeScene(SCENE_COLLISION);
	else if (key == '3')
		ChangeScene(SCENE_GAME);
	else if (key == '4')
		ChangeScene(SCENE_OBJLOADER);

	if (key == 27)
		exit(0);
}