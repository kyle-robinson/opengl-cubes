#pragma once

#include "SceneObject.h"

class Cube : public SceneObject
{
public:
	Cube(Mesh* mesh, float x, float y, float z);
	~Cube();

protected:
	void Draw();
	void Update();
};