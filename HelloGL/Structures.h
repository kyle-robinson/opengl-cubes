#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include <string>

struct Node
{
	std::string data;
	Node *left, *right;
};

enum SCENES
{
	SCENE_MENU = 0,
	SCENE_STARFIELD,
	SCENE_COLLISION,
	SCENE_GAME,
	SCENE_OBJLOADER
};

// CAMERA
struct Vector3
{
	float x;
	float y;
	float z;
};

struct Camera
{
	Vector3 eye;
	Vector3 center;
	Vector3 up;
};

// QUADS
struct Quads
{
	int x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x4, y4, z4;
	float r, g, b;
	int state;
	int total;
};

// MESH
struct Vertex
{
	GLfloat x, y, z;
};

struct Color
{
	GLfloat r, g, b;
};

struct TexCoord
{
	GLfloat u, v;
};

struct Mesh
{
	Vertex* Vertices;
	Vector3* Normals;
	TexCoord* TexCoords;
	GLushort* Indices;
	int VertexCount, NormalCount, IndexCount, TexCoordCount;
};

// LIGHTING
struct Vector4
{
	float x, y, z, w;
};

struct Lighting
{
	Vector4 Ambient, Diffuse, Specular;
};

struct Material
{
	Vector4 Ambient, Diffuse, Specular;
	GLfloat Shininess;
};