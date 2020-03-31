#pragma once

class SceneManager;
class SceneStarfield;
class SceneCollision;
class SceneGame;

namespace GLUTCallbacks
{
	void Init(SceneManager* gl);
	void Init(SceneStarfield* gl);
	void Init(SceneCollision* gl);
	void Init(SceneGame* gl);

	void Display();
	void Timer(int preferredRefresh);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);
}