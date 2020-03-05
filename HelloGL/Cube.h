#pragma once

#include "SceneObject.h"

class Cube : public SceneObject
{
public:
	Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z);
	~Cube();

protected:
	void Draw();
	void Update();
	void SetupLight();

private:
	Material* _material;
};