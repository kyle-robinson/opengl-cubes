#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh)
{
	_mesh = mesh;
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