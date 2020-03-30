#pragma once

#include "SceneObject.h"

class Pyramid : public SceneObject
{
public:
	Pyramid(Mesh* mesh, Texture2D* texture, float x, float y, float z);
	~Pyramid();

protected:
	void Draw();
	void Update();
	void SetupLight();

private:
	Material* _material;
};