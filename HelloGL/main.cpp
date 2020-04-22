#include "SceneManager.h"
#include "Treenode.h"

#include "GL/freeglut.h"
#include <gl/GLU.h>
#include <iostream>

SceneManager* sceneManager;
Treenode* tree;

void CreateSceneGraph();
void InitWindow(int argc, char* argv[]);

int main(int argc, char* argv[])
{	
	CreateSceneGraph();
	InitWindow(argc, argv);
	sceneManager = new SceneManager();
	return 0;
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Project - Showcase of Knowledge");

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 800, 800);
	gluPerspective(90, 1, 1, 1000);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void CreateSceneGraph()
{
	std::cout << "SCENE GRAPH" << std::endl << std::endl;
	
	tree = new Treenode();

	Node* root = tree->newNode("main");
	Node* sceneManager = tree->addChild(root, "SceneManager");
	Node* glutCallbacks = tree->addChild(sceneManager, "GLUTCallbacks");
	Node* scene = tree->addSibling(glutCallbacks, "Scene");

	Node* sceneMenu = tree->addChild(scene, "SceneMenu");
	Node* structs = tree->addSibling(sceneMenu, "Structures");
	
	Node* sceneStarfield = tree->addChild(structs, "SceneStarfield");
	Node* starSceneObject = tree->addChild(sceneStarfield, "SceneObject");
	Node* starTexture2D = tree->addChild(starSceneObject, "Texture2D");
	Node* starCube = tree->addSibling(starTexture2D, "Cube");
	
	Node* sceneCollision = tree->addChild(starCube, "SceneCollision");
	Node* collisionSceneObject = tree->addChild(sceneCollision, "SceneObject");
	Node* collisionTexture2D = tree->addChild(collisionSceneObject, "Texture2D");
	Node* collisionCube = tree->addSibling(collisionTexture2D, "Cube");
	
	Node* sceneGame = tree->addChild(collisionCube, "SceneGame");

	Node* sceneOBJ = tree->addChild(sceneGame, "SceneOBJ");
	Node* modelOBJ = tree->addChild(sceneOBJ, "ModelOBJ");

	tree->traverseTree(root);
}