#pragma once

class Scene
{
public:
	Scene(int argc, char* argv[]);
	~Scene(void);

	virtual void Display();
	virtual void Update();

private:

};