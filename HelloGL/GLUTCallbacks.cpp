#include "GLUTCallbacks.h"

#include "SceneManager.h"
#include "SceneMenu.h"

#include "SceneStarfield.h"
#include "SceneCollision.h"
#include "SceneGame.h"
#include "SceneOBJ.h"

namespace GLUTCallbacks
{
	namespace
	{
		SceneManager* sceneManager = nullptr;
		SceneMenu* sceneMenu = nullptr;

		SceneStarfield* sceneStarfield = nullptr;
		SceneCollision* sceneCollision = nullptr;
		SceneGame* sceneGame = nullptr;
		SceneOBJ* sceneOBJ = nullptr;
	}

	void Init(SceneManager *gl)
	{
		sceneManager = gl;
	}

	void Init(SceneMenu *gl)
	{
		sceneStarfield = nullptr;
		sceneCollision = nullptr;
		sceneGame = nullptr;
		sceneOBJ = nullptr;
		sceneMenu = gl;
	}

	void Init(SceneStarfield *gl)
	{
		sceneMenu = nullptr;
		sceneCollision = nullptr;
		sceneGame = nullptr;
		sceneOBJ = nullptr;
		sceneStarfield = gl;
	}

	void Init(SceneCollision *gl)
	{
		sceneMenu = nullptr;
		sceneStarfield = nullptr;
		sceneGame = nullptr;
		sceneOBJ = nullptr;
		sceneCollision = gl;
	}

	void Init(SceneGame *gl)
	{
		sceneMenu = nullptr;
		sceneStarfield = nullptr;
		sceneCollision = nullptr;
		sceneOBJ = nullptr;
		sceneGame = gl;
	}

	void Init(SceneOBJ* gl)
	{
		sceneMenu = nullptr;
		sceneStarfield = nullptr;
		sceneCollision = nullptr;
		sceneGame = nullptr;
		sceneOBJ = gl;
	}

	void Display()
	{
		if (sceneMenu != nullptr)
			sceneMenu->Display();
		else if (sceneStarfield != nullptr)
			sceneStarfield->Display();
		else if (sceneCollision != nullptr)
			sceneCollision->Display();
		else if (sceneGame != nullptr)
			sceneGame->Display();
		else if (sceneOBJ != nullptr)
			sceneOBJ->Display();
	}

	void Timer(int preferredRefresh)
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);

		if (sceneMenu != nullptr)
			sceneMenu->Update();
		else if (sceneStarfield != nullptr)
			sceneStarfield->Update();
		else if (sceneCollision != nullptr)
			sceneCollision->Update();
		else if (sceneGame != nullptr)
			sceneGame->Update();
		else if (sceneOBJ != nullptr)
			sceneOBJ->Update();

		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;
		glutTimerFunc(preferredRefresh - updateTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		sceneManager->Keyboard(key, x, y);

		if (sceneStarfield != nullptr)
			sceneStarfield->Keyboard(key, x, y);
		else if (sceneCollision != nullptr)
			sceneCollision->Keyboard(key, x, y);
		else if (sceneGame != nullptr)
			sceneGame->Keyboard(key, x, y);
		else if (sceneOBJ != nullptr)
			sceneOBJ->Keyboard(key, x, y);
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		if (sceneStarfield != nullptr)
			sceneStarfield->KeyboardUp(key, x, y);
		else if (sceneCollision != nullptr)
			sceneCollision->KeyboardUp(key, x, y);
		else if (sceneGame != nullptr)
			sceneGame->KeyboardUp(key, x, y);
	}

	void KeyboardSpecial(int key, int x, int y)
	{	
		if (sceneStarfield != nullptr)
			sceneStarfield->KeyboardSpecial(key, x, y);
		else if (sceneCollision != nullptr)
			sceneCollision->KeyboardSpecial(key, x, y);
		else if (sceneGame != nullptr)
			sceneGame->KeyboardSpecial(key, x, y);
		else if (sceneOBJ != nullptr)
			sceneOBJ->KeyboardSpecial(key, x, y);
	}

	void KeyboardSpecialUp(int key, int x, int y)
	{
		if (sceneCollision != nullptr)
			sceneCollision->KeyboardSpecialUp(key, x, y);
	}

	void ObjectMenu(int value)
	{
		if (sceneOBJ != nullptr)
			sceneOBJ->ObjectMenu(value);
	}

	void ColourMenu(int value)
	{
		if (sceneOBJ != nullptr)
			sceneOBJ->ColourMenu(value);
	}
}