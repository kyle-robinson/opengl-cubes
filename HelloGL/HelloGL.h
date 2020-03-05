#pragma once

#include "GLUTCallbacks.h"
#include "Cube.h"
#include "Pyramid.h"
#include "SceneObject.h"

#define REFRESHRATE		16
#define OBJECTCOUNT		500

class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL(void);
	void Display();
	void Update();

	void InitObjects();
	void InitGL(int argc, char* argv[]);
	void InitLighting(int argc, char* argv[]);

	void Keyboard(unsigned char key, int x, int y);

private:
	float rotation;

	Camera* camera;
	SceneObject* objects[OBJECTCOUNT];

	Vector4 * _lightPosition;
	Lighting* _lightData;
};