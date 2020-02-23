#include "Cube.h"

#include <cstdlib>
#include <ctime>

Cube::Cube(Mesh* mesh, Texture2D* texture, float x, float y, float z) : SceneObject(mesh, texture)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_rotation = 0.0f;
}

Cube::~Cube()
{
	delete _mesh;
	_mesh = NULL;
}

void Cube::Draw()
{
	if (_mesh->Vertices != nullptr && _mesh->Colors != nullptr && _mesh->Indices != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		glColorPointer(3, GL_FLOAT, 0, _mesh->Colors);
		glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);

		glPushMatrix();
			glTranslatef(_position.x, _position.y, _position.z);
			glRotatef(_rotation, 1.0f, 0.0f, 0.0f);
			glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Cube::Update()
{
	srand((unsigned int)time(NULL));
	float randRotate = 5.0f;
	_rotation += (float(rand()) / float((RAND_MAX)) * randRotate);

	srand((unsigned int)time(NULL));
	float randPosition = 0.2f;
	_position.z += (float(rand()) / float((RAND_MAX)) * randPosition);
}