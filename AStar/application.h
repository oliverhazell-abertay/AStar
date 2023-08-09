#pragma once
#include <SFML/Graphics.hpp>
#include "node.h"
#include "priorityQueue.h"
#include <list>
#include <set>
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

using namespace std;

typedef pair<int, int> Pair;
typedef pair<int, Pair> pPair;

class Application
{
public:
	void Init(sf::RenderWindow*);
	void CleanUp();
	int Update();
	void Render();
	void InitGrid();
	void FindGridSquareCollision(sf::RenderWindow*, int);
	bool FindPath();
	void ColourPath(); 


	// Grid consts
	const static int gridWidth = 19;
	const static int gridHeight = 11;
	const float shapeWidth = WINDOW_WIDTH / gridWidth;
	const float shapeHeight = WINDOW_HEIGHT / gridHeight;
	// Path
	bool findingPath = false;
	vector<Node*> path;
	Node* startNode;
	Node* endNode;
	// Open and closed lists
	set <pPair> openNodes;
	bool closedNodes[gridWidth][gridHeight];
	// Grid array
	Node grid[gridWidth][gridHeight];
	// SFML
	sf::RenderWindow* window;
	sf::Font myFont;
};
