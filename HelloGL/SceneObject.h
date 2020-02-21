#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "Structures.h"

class SceneObject
{
protected:
	Mesh* _mesh;
	Vector3 _position;
	GLfloat _rotation;

public:
	SceneObject(Mesh* mesh);
	virtual ~SceneObject();

	virtual void Update();
	virtual void Draw();
};