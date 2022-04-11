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

	const static int gridWidth = 5;
	const static int gridHeight = 4;
	const float shapeWidth = WINDOW_WIDTH / gridWidth;
	const float shapeHeight = WINDOW_HEIGHT / gridHeight;
	Node grid[gridWidth][gridHeight];
	Node* startNode;
	Node* endNode;
	sf::RenderWindow* window;
	sf::Font myFont;
};
