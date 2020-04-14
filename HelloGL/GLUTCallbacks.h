#pragma once

class SceneManager;
class SceneMenu;

class SceneStarfield;
class SceneCollision;
class SceneGame;
class SceneOBJ;

namespace GLUTCallbacks
{
	void Init(SceneManager* gl);
	void Init(SceneMenu* gl);

	void Init(SceneStarfield* gl);
	void Init(SceneCollision* gl);
	void Init(SceneGame* gl);
	void Init(SceneOBJ* gl);

	void Display();
	void Timer(int preferredRefresh);
	
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

	void KeyboardSpecial(int key, int x, int y);
	void KeyboardSpecialUp(int key, int x, int y);
}