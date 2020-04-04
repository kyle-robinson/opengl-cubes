#include "ModelOBJ.h"

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
using namespace std;

ModelOBJ::ModelOBJ()
{
	this->totalTriangles = 0;
	this->totalPoints = 0;
}

float* ModelOBJ::calculateNormal(float* coord1, float* coord2, float* coord3)
{
	// Calculate Vec1 and Vec2
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	// Cross Product
	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	// Normalisation Factor
	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;

	return norm;
}

int ModelOBJ::Load(char* filename)
{
	string line;
	ifstream objFile(filename);
	if (objFile.is_open())
	{
		objFile.seekg(0, ios::end);
		long fileSize = objFile.tellg();
		objFile.seekg(0, ios::beg);

		vertexBuffer = (float*)malloc(fileSize);
		triangleFaces = (float*)malloc(fileSize * sizeof(float));
		normals = (float*)malloc(fileSize * sizeof(float));

		int triangle_index = 0;
		int normal_index = 0;

		while (!objFile.eof())
		{
			getline(objFile, line);

			if (line.c_str()[0] == 'v')
			{
				line[0] = ' ';

				sscanf_s(line.c_str(), "%f %f %f ",
					&vertexBuffer[totalPoints],
					&vertexBuffer[totalPoints + 1],
					&vertexBuffer[totalPoints + 2]);

				totalPoints += POINTS_PER_VERTEX;
			}
			if (line.c_str()[0] == 'f')
			{
				line[0] = ' ';

				int vertexNumber[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%i%i%i",
					&vertexNumber[0],
					&vertexNumber[1],
					&vertexNumber[2]);

				vertexNumber[0] -= 1;
				vertexNumber[1] -= 1;
				vertexNumber[2] -= 1;

				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					triangleFaces[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					triangleFaces[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					triangleFaces[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					tCounter += POINTS_PER_VERTEX;
				}

				float coord1[3] = { triangleFaces[triangle_index], triangleFaces[triangle_index + 1],triangleFaces[triangle_index + 2] };
				float coord2[3] = { triangleFaces[triangle_index + 3],triangleFaces[triangle_index + 4],triangleFaces[triangle_index + 5] };
				float coord3[3] = { triangleFaces[triangle_index + 6],triangleFaces[triangle_index + 7],triangleFaces[triangle_index + 8] };
				float* norm = this->calculateNormal(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter] = norm[0];
					normals[normal_index + tCounter + 1] = norm[1];
					normals[normal_index + tCounter + 2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				totalTriangles += TOTAL_FLOATS_IN_TRIANGLE;
			}
		}
		objFile.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
	return 0;
}

void ModelOBJ::Release()
{
	free(this->triangleFaces);
	free(this->normals);
	free(this->vertexBuffer);
}

void ModelOBJ::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, triangleFaces);
		glNormalPointer(GL_FLOAT, 0, normals);
		glDrawArrays(GL_TRIANGLES, 0, totalTriangles);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}