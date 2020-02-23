#include "Texture2D.h"

#include <iostream>
#include <fstream>

using namespace std;

Texture2D::Texture2D()
{

}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &_ID);
}

bool Texture2D::Load(char* path, int width, int height)
{
	char* tempTextureData;
	int fileSize;
	ifstream inFile;

	_width = width;
	_height = height;

	inFile.open(path, ios::binary);
	if (!inFile.good())
	{
		cerr << "Can't open texture file " << path << endl;
		return false;
	}

	// Get the total file size
	inFile.seekg(0, ios::end);
	fileSize = (int)inFile.tellg();
	
	// Store the data in an array
	tempTextureData = new char[fileSize];
	
	// Reset position - read all data and store in array
	inFile.seekg(0, ios::beg);
	inFile.read(tempTextureData, fileSize);
	inFile.close();

	cout << path << " loaded." << endl;

	// Get the Texture ID and bind the Texture to the ID
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	// Clear up the data
	delete[] tempTextureData;
	return true;
}