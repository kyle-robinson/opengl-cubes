#pragma once

#include "Structures.h"

class SceneObject
{
protected:
	Mesh* _mesh;

public:
	SceneObject(Mesh* mesh);
	virtual ~SceneObject();

	virtual void Update();
	virtual void Draw();
};