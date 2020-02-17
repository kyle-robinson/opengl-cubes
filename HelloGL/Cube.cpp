#include "Cube.h"

Vertex* Cube::indexedVertices;
Color* Cube::indexedColors;
GLushort* Cube::indices;

int Cube::numVertices = 0;
int Cube::numColors = 0;
int Cube::numIndices = 0;

/*GLushort Cube::indices[] =
{
	0, 1, 2,	2, 3, 0,	// front
	0, 3, 4,	4, 5, 0,	// right
	0, 5, 6,	6, 1, 0,	// top
	1, 6, 7,	7, 2, 1,	// left
	7, 4, 3,	3, 2, 7,	// bottom
	4, 7, 6,	6, 5, 4		// back
};*/

Cube::Cube(float x, float y, float z)
{
	_rotation = 0.1f;

	_position.x = x;
	_position.y = y;
	_position.z = z;
}

Cube::~Cube()
{
	delete indexedVertices;
	indexedVertices = NULL;
	
	delete indexedColors;
	indexedColors = NULL;
}

void Cube::Draw()
{
	if (indexedVertices != nullptr && indexedColors != nullptr && indices != nullptr)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, indexedVertices);
		glColorPointer(3, GL_FLOAT, 0, indexedColors);

		glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, 1.0f, 0.0f, 0.0f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
		glPopMatrix();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void Cube::Update()
{
	_rotation += 5.0f;
	_position.z += 0.1f;
}

void Cube::InputVertex(Vertex &v, std::ifstream &in)
{
	in >> v.x;
	in >> v.y;
	in >> v.z;
}

void Cube::InputColor(Color &c, std::ifstream &in)
{
	in >> c.r;
	in >> c.g;
	in >> c.b;
}

bool Cube::Load(char* path)
{	
	std::ifstream inFile;
	inFile.open(path);
	if (!inFile.good())
	{
		std::cerr << "Can't open text file " << path << std::endl;
		return false;
	}

	// Load vertex information
	inFile >> numVertices;
	indexedVertices = new (nothrow) Vertex[numVertices];

	if (indexedVertices == nullptr)
	{
		std::cerr << ("Vertices error") << std::endl;
	}

	for (int i = 0; i < numVertices; i++)
	{
		InputVertex(indexedVertices[i], inFile);
	}

	// Load colour information
	inFile >> numColors;
	indexedColors = new (nothrow) Color[numColors];

	if (indexedColors == nullptr)
	{
		std::cerr << ("Colors error") << std::endl;
	}

	for (int i = 0; i < numColors; i++)
	{
		InputColor(indexedColors[i], inFile);
	}

	// Load index information
	inFile >> numIndices;
	indices = new (nothrow) GLushort[numIndices];

	if (indices == nullptr)
	{
		std::cerr << ("Indices error") << std::endl;
	}

	for (int i = 0; i < numIndices; i++)
	{
		inFile >> indices[i];
	}

	inFile.close();
	return true;
}