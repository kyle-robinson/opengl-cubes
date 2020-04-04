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
	int Load(char* filename);
	void Draw();
	void Release();

	float* normals;
	float* triangleFaces;
	float* vertexBuffer;

	long totalPoints;
	long totalTriangles;
};