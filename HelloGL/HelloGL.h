#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"

#include "GLUTCallbacks.h"
#include "SceneObject.h"
#include "Cube.h"
#include "Pyramid.h"

#include <iostream>
#include <fstream>

#define REFRESHRATE		16
#define OBJECTCOUNT		1000

class HelloGL
{
public:
	// Default Functions
	HelloGL(int argc, char* argv[]);
	~HelloGL(void);
	void Display();
	void Update();

	// OpenGL Initialisation
	void InitObjects();
	void InitGL(int argc, char* argv[]);
	
	// Keyboard Input
	void Keyboard(unsigned char key, int x, int y);

private:
	// Rotation Variables
	float rotation;
	
	// Pointers
	Camera* camera;
	SceneObject* objects[OBJECTCOUNT];
};