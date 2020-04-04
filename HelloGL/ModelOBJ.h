#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

class ModelOBJ
{
public:
	ModelOBJ();
	float* calculateNormal(float* coord1, float* coord2, float* coord3);
	int Load(char* filename);	// Loads the model
	void Draw();					// Draws the model on the screen
	void Release();				// Release the model

	float* normals;							// Stores the normals
	float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles
};