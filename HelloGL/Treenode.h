#pragma once

#include "Structures.h"
#include <string>

#define WIDTH	4

class Treenode
{
public:
	Treenode();
	~Treenode();

	Node* newNode(std::string data);
	Node* addSibling(Node* n, std::string data);
	Node* addChild(Node* n, std::string data);
	void traverseTree(Node* root);
	
private:
	bool treeStarted, treeFinished;
};