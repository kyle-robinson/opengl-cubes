#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"
#include "Structures.h"
#include <iostream>
#include <fstream>

#define CUBECOUNT 200

using namespace std;

class Cube
{
private:
	static Vertex* indexedVertices;
	static Color* indexedColors;
	static GLushort* indices;

	//static Vertex indexedVertices[];
	//static Color indexedColors[];
	//static GLushort indices[];

	//Mesh* _mesh;

	GLfloat _rotation;
	Vector3 _position;

	static int numVertices, numColors, numIndices;

public:
	Cube(float x, float y, float z);
	~Cube();

	void Draw();
	void Update();

	static bool Load(char* path);
	static void InputVertex(Vertex &v, std::ifstream &in);
	static void InputColor(Color &c, std::ifstream &in);

	//Cube(Mesh* mesh, float x, float y, float z);
};