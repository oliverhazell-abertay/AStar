#include "node.h"

Node::Node()
{
}

void Node::Init(sf::Vector2f initWorldPos, sf::Vector2f initSize, sf::Font* myFont)
{
	worldPos = initWorldPos;
	size = initSize;
	font = myFont;
	gCostText.setFont(*font);
	gCostText.setString("10");
	gCostText.setFillColor(sf::Color::Black);
}

void Node::Update()
{
	shape.setSize(size);
	shape.setPosition(worldPos);
	gCostText.setPosition(sf::Vector2f(worldPos));
}

void Node::Render(sf::RenderWindow* window)
{
	window->draw(shape);
	window->draw(gCostText);
}
