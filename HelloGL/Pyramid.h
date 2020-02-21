#pragma once

#include "SceneObject.h"

class Pyramid : public SceneObject
{
public:
	Pyramid(Mesh* mesh, float x, float y, float z);
	~Pyramid();

protected:
	virtual void Draw();
	virtual void Update();
};