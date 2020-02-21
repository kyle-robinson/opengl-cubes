#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "Structures.h"
#include "SceneObject.h"

#include <iostream>
#include <fstream>

using namespace std;

class MeshDraw : public SceneObject
{
private:
	//Mesh* _mesh;
	
	//Vector3 _position;
	//GLfloat _rotation;

public:
	MeshDraw(Mesh* mesh, float x, float y, float z);
	~MeshDraw();

	void Draw();
	void Update();
};