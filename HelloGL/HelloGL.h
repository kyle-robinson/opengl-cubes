#pragma once
#include "GLUTCallbacks.h"
#include "Cube.h"

#define REFRESHRATE 16

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
	float rotationRect;
	float rotationSquare;
	float rotationTriangle;
	
	// Pointers
	Camera* camera;
	Cube* cube[CUBECOUNT];
};