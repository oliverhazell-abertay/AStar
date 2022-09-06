#pragma once
#include <SFML/Graphics.hpp>
#include "node.h"
#include <list>
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

class Application
{
public:
	void Init(sf::RenderWindow*);
	void CleanUp();
	int Update();
	void Render();
	void InitGrid();
	void FindGridSquareCollision(sf::RenderWindow*, int);
	void FindPath();
	bool CompareF(Node*, Node*);

	const static int gridWidth = 19;
	const static int gridHeight = 11;
	const float shapeWidth = WINDOW_WIDTH / gridWidth;
	const float shapeHeight = WINDOW_HEIGHT / gridHeight;
	bool findingPath = false;
	std::vector<Node*> path;
	std::list<Node*> openNodes;
	std::list<Node*> closedNodes;
	Node grid[gridWidth][gridHeight];
	Node* startNode;
	Node* endNode;
	sf::RenderWindow* window;
	sf::Font myFont;
};
