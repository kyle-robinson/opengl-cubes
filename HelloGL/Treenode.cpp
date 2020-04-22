#include "Treenode.h"
#include <iostream>
#include <iomanip>

Treenode::Treenode()
{
	treeStarted = treeFinished = false;
}

Treenode::~Treenode()
{

}

Node* Treenode::newNode(std::string data)
{
	Node* newNode = new Node;
	newNode->left = newNode->right = NULL;
	newNode->data = data;
	return newNode;
}

Node* Treenode::addSibling(Node* n, std::string data)
{
	if (n == NULL)
		return NULL;

	while (n->left)
		n = n->left;

	return (n->left = newNode(data));
}

Node* Treenode::addChild(Node* n, std::string data)
{
	if (n == NULL)
		return NULL;

	if (n->right)
		return addSibling(n->right, data);
	else
		return (n->right = newNode(data));
}

void Treenode::traverseTree(Node* root)
{
	if (root == NULL)
		return;

	if (!treeStarted)
	{
		std::cout << "START";
		treeStarted = true;
	}

	while (root)
	{
		std::cout << std::setw(WIDTH) << "->" << root->data;
		if (root->right)
			traverseTree(root->right);
		root = root->left;
	}

	if (!treeFinished)
	{
		std::cout << std::setw(WIDTH*2) << "->END" << std::endl << std::endl;
		treeFinished = true;
	}
}