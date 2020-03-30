#include "Pyramid.h"

Pyramid::Pyramid(Mesh* mesh, Texture2D* texture, float x, float y, float z) : SceneObject(mesh, texture)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

Pyramid::~Pyramid()
{
	delete _material;
	_material = NULL;
}

void Pyramid::Draw()
{
	if (_mesh->Vertices != nullptr && _mesh->Normals != nullptr && _mesh->Indices != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());

		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnable(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
		//glColorPointer(3, GL_FLOAT, 0, _mesh->Normals);
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
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Pyramid::Update()
{
	SceneObject::Update();
}

void Pyramid::SetupLight()
{
	_material = new Material();

	_material->Ambient.x = red;
	_material->Ambient.y = green;
	_material->Ambient.z = blue;
	_material->Ambient.w = 1.0;

	_material->Diffuse.x = red;
	_material->Diffuse.y = green;
	_material->Diffuse.z = blue;
	_material->Diffuse.w = 1.0;

	_material->Specular.x = 1.0;
	_material->Specular.y = 1.0;
	_material->Specular.z = 1.0;
	_material->Specular.w = 1.0;

	_material->Shininess = 100.0f;
}