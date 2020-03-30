#pragma once

#include <gl\GL.h>

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