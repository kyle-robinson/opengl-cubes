#include "MeshDraw.h"

MeshDraw::MeshDraw(Mesh* mesh, float x, float y, float z) : SceneObject(mesh)
{
	//_mesh = mesh;

	_rotation = 0.0f;

	_position.x = x;
	_position.y = y;
	_position.z = z;
}

MeshDraw::~MeshDraw()
{
	//delete _mesh->Vertices;
	//_mesh->Vertices = NULL;
	//
	//delete _mesh->Colors;
	//_mesh->Colors = NULL;
	//
	//delete _mesh->Indices;
	//_mesh->Indices = NULL;
	//
	//delete _mesh;
	//_mesh = NULL;
}

void MeshDraw::Draw()
{
	if (_mesh->Vertices != nullptr && _mesh->Colors != nullptr && _mesh->Indices != nullptr)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		glColorPointer(3, GL_FLOAT, 0, _mesh->Colors);

		glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, 1.0f, 0.0f, 0.0f);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
		glPopMatrix();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void MeshDraw::Update()
{
	_rotation += 5.0f;
	_position.z += 0.1f;
}