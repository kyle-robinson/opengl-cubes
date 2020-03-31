#pragma once

class Scene
{
public:
	Scene();
	~Scene(void);

	virtual void Display();
	virtual void Update();
};