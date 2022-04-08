#pragma once
#include <SFML/Graphics.hpp>

class Node
{
public:
	Node();
	void Update();

	sf::Vector2f GetWorldPos() { return worldPos; }
	sf::Vector2f GetSize() { return size; }
	void SetWorldPos(sf::Vector2f nextWorldPos) { worldPos = nextWorldPos; }
	void SetSize(sf::Vector2f nextSize) { size = nextSize; }
	// Neighbours
	Node* northAdj;
	Node* eastAdj;
	Node* southAdj;
	Node* westAdj;
	// Shape
	sf::RectangleShape shape;

private:
	sf::Vector2f worldPos;
	sf::Vector2f size;
};

