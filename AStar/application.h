#pragma once
#include <SFML/Graphics.hpp>
#include "node.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Application
{
public:
	void Init(sf::RenderWindow*);
	void CleanUp();
	int Update();
	void Render();
	void InitGrid();
	void FindGridSquareCollision(sf::RenderWindow*, bool);
	void FindPath();

	const static int gridWidth = 5;
	const static int gridHeight = 4;
	const float shapeWidth = WINDOW_WIDTH / gridWidth;
	const float shapeHeight = WINDOW_HEIGHT / gridHeight;
	bool findingPath = false;
	std::vector<Node*> path;
	std::vector<Node*> visitedNodes;
	std::vector<Node*> examinedNodes;
	Node grid[gridWidth][gridHeight];
	Node* startNode;
	Node* endNode;
	sf::RenderWindow* window;
	sf::Font myFont;
};
