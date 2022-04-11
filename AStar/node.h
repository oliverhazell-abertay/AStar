#pragma once
#include <SFML/Graphics.hpp>

class Node
{
public:
	Node();
	void Init(sf::Vector2f, sf::Vector2f, sf::Vector2i, sf::Font*);
	void Update(Node*, Node*);
	void Render(sf::RenderWindow* window);

	sf::Vector2f GetWorldPos() { return worldPos; }
	sf::Vector2f GetSize() { return size; }
	void SetWorldPos(sf::Vector2f nextWorldPos) { worldPos = nextWorldPos; }
	void SetSize(sf::Vector2f nextSize) { size = nextSize; }
	// Neighbours
	Node* northAdj;
	Node* eastAdj;
	Node* southAdj;
	Node* westAdj;
	Node* northWestAdj;
	Node* northEastAdj;
	Node* southWestAdj;
	Node* southEastAdj;
	// Shape
	sf::RectangleShape shape;
	// Font
	sf::Font* font;
	// Position in array
	sf::Vector2i gridPos;
	// Calculate gCost, hCost, fCost
	int CalculateG(Node*);
	int CalculateH(Node*);
private:
	sf::Vector2f worldPos;
	sf::Vector2f size;
	// fgh costs
	int fCost, gCost, hCost;
	// Text
	sf::Text gCostText, hCostText, fCostText;
};

