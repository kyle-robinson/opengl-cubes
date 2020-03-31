#include "GLUTCallbacks.h"

#include "SceneManager.h"
#include "SceneStarfield.h"
#include "SceneCollision.h"
#include "SceneGame.h"

namespace GLUTCallbacks
{
	namespace
	{
		SceneManager* sceneManager = nullptr;
		SceneStarfield* sceneStarfield = nullptr;
		SceneCollision* sceneCollision = nullptr;
		SceneGame* sceneGame = nullptr;
	}

	void Init(SceneManager *gl)
	{
		sceneManager = gl;
	}

	void Init(SceneStarfield *gl)
	{
		sceneCollision = nullptr;
		sceneGame = nullptr;
		sceneStarfield = gl;
	}

	void Init(SceneCollision *gl)
	{
		sceneStarfield = nullptr;
		sceneGame = nullptr;
		sceneCollision = gl;
	}

	void Init(SceneGame *gl)
	{
		sceneStarfield = nullptr;
		sceneCollision = nullptr;
		sceneGame = gl;
	}

	void Display()
	{
		if (sceneStarfield != nullptr)
		{
			sceneStarfield->Display();
		}
		else if (sceneCollision != nullptr)
		{
			sceneCollision->Display();
		}
		else if (sceneGame != nullptr)
		{
			sceneGame->Display();
		}
	}

	void Timer(int preferredRefresh)
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		if (sceneStarfield != nullptr)
		{
			sceneStarfield->Update();
		}
		else if (sceneCollision != nullptr)
		{
			sceneCollision->Update();
		}
		else if (sceneGame != nullptr)
		{
			sceneGame->Update();
		}
		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;
		glutTimerFunc(preferredRefresh - updateTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		sceneManager->Keyboard(key, x, y);
		
		if (sceneStarfield != nullptr)
		{
			sceneStarfield->Keyboard(key, x, y);
		}
		if (sceneCollision != nullptr)
		{
			sceneCollision->Keyboard(key, x, y);
		}
		if (sceneGame != nullptr)
		{
			sceneGame->Keyboard(key, x, y);
		}
	}

	void KeyboardSpecial(int key, int x, int y)
	{	
		if (sceneStarfield != nullptr)
		{
			sceneStarfield->KeyboardSpecial(key, x, y);
		}
		if (sceneCollision != nullptr)
		{
			sceneCollision->KeyboardSpecial(key, x, y);
		}
		if (sceneGame != nullptr)
		{
			sceneGame->KeyboardSpecial(key, x, y);
		}
	}
}