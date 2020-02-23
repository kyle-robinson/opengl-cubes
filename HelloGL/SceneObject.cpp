#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture)
{
	_mesh = mesh;
	_texture = texture;
}

SceneObject::~SceneObject()
{
	delete _mesh;
	_mesh = NULL;
}

void SceneObject::Draw()
{

}

void SceneObject::Update()
{

}