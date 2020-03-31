#pragma once

#include "GLUTCallbacks.h"
#include "Scene.h"

#include "Cube.h"
#include "Pyramid.h"
#include "SceneObject.h"

#define REFRESHRATE		16
#define OBJECTCOUNT		500
#define MOVEMENT_SPEED	0.1f

class SceneStarfield : Scene
{
public:
	SceneStarfield(int argc, char* argv[]);
	~SceneStarfield(void);
	void Display();
	void Update();

	void InitGL(int argc, char* argv[]);
	void InitLighting(int argc, char* argv[]);
	void InitObjects();

	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);
	void DrawString(const char* text, Vector3* position, Color* color);

private:
	float rotation;

	Mesh* cubeMesh;
	Mesh* pyramidMesh;
	Texture2D* texturePenguins;
	Texture2D* textureStars;

	Camera* camera;
	SceneObject* objects[OBJECTCOUNT];

	Vector4 * _lightPosition;
	Lighting* _lightData;
};