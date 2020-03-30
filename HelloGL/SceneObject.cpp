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
	srand((unsigned int)time(NULL));
	float randRotate = 5.0f;
	_rotation += (float(rand()) / float((RAND_MAX)) * randRotate);

	srand((unsigned int)time(NULL));
	float randPosition = 0.2f;
	_position.z += (float(rand()) / float((RAND_MAX)) * randPosition);
}