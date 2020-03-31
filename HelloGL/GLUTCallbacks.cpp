#include "GLUTCallbacks.h"
#include "SceneStarfield.h"

namespace GLUTCallbacks
{
	namespace
	{
		SceneStarfield* sceneStarfield = nullptr;
	}

	void Init(SceneStarfield *gl)
	{
		sceneStarfield = gl;
	}

	void Display()
	{
		if (sceneStarfield != nullptr)
		{
			sceneStarfield->Display();
		}
	}

	void Timer(int preferredRefresh)
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		sceneStarfield->Update();
		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;
		glutTimerFunc(preferredRefresh - updateTime, GLUTCallbacks::Timer, preferredRefresh);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		sceneStarfield->Keyboard(key, x, y);
	}

	void KeyboardSpecial(int key, int x, int y)
	{
		sceneStarfield->KeyboardSpecial(key, x, y);
	}
}