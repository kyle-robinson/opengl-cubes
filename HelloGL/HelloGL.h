#pragma once
#include "GLUTCallbacks.h"
#include "MeshDraw.h"

#define REFRESHRATE		16
#define CUBECOUNT		100
#define PYRAMIDCOUNT	100
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

	//MeshDraw* cube[CUBECOUNT];
	//MeshDraw* pyramid[PYRAMIDCOUNT];
};