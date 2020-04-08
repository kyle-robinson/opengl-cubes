#include "SceneObject.h"

#include <cstdlib>
#include <ctime>

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture)
{
	_mesh = mesh;
	_texture = texture;

	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;

	_rotation = 0.0f;
	_radius = 1.0f;
}

SceneObject::~SceneObject()
{
	delete _mesh;
	_mesh = NULL;

	delete _texture;
	_texture = NULL;
}

void SceneObject::Draw()
{

}

void SceneObject::Update()
{
	
}