#pragma once

class SceneStarfield;

namespace GLUTCallbacks
{
	void Init(SceneStarfield* gl);
	void Display();
	void Timer(int preferredRefresh);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);
}