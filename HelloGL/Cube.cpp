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

	delete _material;
	_material = NULL;
}

void Cube::Draw()
{
	if (_mesh->Vertices != nullptr && _mesh->Normals != nullptr && _mesh->Indices != nullptr)
	{		
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		glNormalPointer(GL_FLOAT, 0, _mesh->Normals);
		glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);

		SetupLight();
		glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->Ambient.x));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->Diffuse.x));
		glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->Specular.x));
		glMaterialf(GL_FRONT, GL_SHININESS, _material->Shininess);

		glPushMatrix();
			glTranslatef(_position.x, _position.y, _position.z);
			glRotatef(_rotation, 1.0f, 0.0f, 0.0f);
			glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Cube::Update()
{
	//srand((unsigned int)time(NULL));
	//float randRotate = 5.0f;
	//_rotation += (float(rand()) / float((RAND_MAX)) * randRotate);

	srand((unsigned int)time(NULL));
	float randPosition = 2.5f;
	_position.z += (float(rand()) / float((RAND_MAX)) * randPosition);
}

void Cube::SetupLight()
{
	_material = new Material();

	_material->Ambient.x = 0.8;
	_material->Ambient.y = 0.05;
	_material->Ambient.z = 0.05;
	_material->Ambient.w = 1.0;

	_material->Diffuse.x = 0.8;
	_material->Diffuse.y = 0.05;
	_material->Diffuse.z = 0.05;
	_material->Diffuse.w = 1.0;

	_material->Specular.x = 1.0;
	_material->Specular.y = 1.0;
	_material->Specular.z = 1.0;
	_material->Specular.w = 1.0;
	
	_material->Shininess = 100.0f;
}