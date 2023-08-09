#pragma once
#include <SFML/Graphics.hpp>

class Node
{
public:
	Node();
	void Init(sf::Vector2f, sf::Vector2f, sf::Vector2i, sf::Font*);
	void Update(Node*, Node*);
	void Render(sf::RenderWindow* window);

	// Position in array
	sf::Vector2i gridPos;
	// Getters and Setters
	sf::Vector2f GetWorldPos() { return worldPos; }
	sf::Vector2f GetSize() { return size; }
	int GetFCost() { return fCost; }
	int GetGCost() { return gCost; }
	int GetHCost() { return hCost; }
	inline void SetGCost(int nextG) { gCost = nextG; }
	void SetWorldPos(sf::Vector2f nextWorldPos) { worldPos = nextWorldPos; }
	void SetSize(sf::Vector2f nextSize) { size = nextSize; }
	// Neighbours 
	std::vector<sf::Vector2i> neighboursOrth;
	std::vector<sf::Vector2i> neighboursDiag;
	int parentX, parentY;
	// Shape
	sf::RectangleShape shape;
	// Font
	sf::Font* font;
	// Calculate gCost, hCost, fCost
	void CalculateFGH(Node*, Node*);
	int CalculateF();
	int CalculateG(Node*);
	int CalculateH(Node*);
	// Flags
	bool obstacle;
private:
	sf::Vector2f worldPos;
	sf::Vector2f size;
	// fgh costs
	int fCost, gCost, hCost;
	// Text
	sf::Text gCostText, hCostText, fCostText;
};

