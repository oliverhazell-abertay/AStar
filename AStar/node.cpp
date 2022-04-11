#include "node.h"

Node::Node()
{
}

void Node::Init(sf::Vector2f initWorldPos, sf::Vector2f initSize, sf::Vector2i gridPosition, sf::Font* myFont)
{
	worldPos = initWorldPos;
	size = initSize;
	gridPos = gridPosition;
	font = myFont;
	// Init gCost text
	gCostText.setFont(*font);
	gCostText.setString(std::to_string(gridPos.x));
	gCostText.setFillColor(sf::Color::Black);
	sf::Vector2f tempPos = worldPos;
	tempPos.x += 10.0f;
	tempPos.y += 10.0f;
	gCostText.setPosition(sf::Vector2f(tempPos));
	// Init hCost text
	hCostText.setFont(*font);
	hCostText.setString(std::to_string(gridPos.x));
	hCostText.setFillColor(sf::Color::Black);
	tempPos.x += 200.0f;
	hCostText.setPosition(sf::Vector2f(tempPos));
	// Init fCost text
	fCostText.setFont(*font);
	fCostText.setString(std::to_string(gridPos.x));
	fCostText.setFillColor(sf::Color::Black);
	tempPos.x = worldPos.x + (size.x * 0.45f);
	tempPos.y = worldPos.y + (size.y * 0.5f);
	fCostText.setPosition(sf::Vector2f(tempPos));
}

void Node::Update(Node* startNode, Node* endNode)
{
	shape.setSize(size);
	shape.setPosition(worldPos);
	// Clear colour
	shape.setFillColor(sf::Color::White);
	// Calculate gCost
	if (startNode)
	{
		// If this is the start node, colour green
		if (startNode->gridPos == gridPos)
		{
			shape.setFillColor(sf::Color::Green);
			gCostText.setString("");
			hCostText.setString("");
			fCostText.setString("A");
		}
		// If not, calculate distance to start node
		else
			gCost = CalculateG(startNode);
	}
	// Calculate hCost
	if (endNode)
	{
		// If this is the end node, colour red
		if (endNode->gridPos == gridPos)
		{
			shape.setFillColor(sf::Color::Red);
			gCostText.setString("");
			hCostText.setString("");
			fCostText.setString("B");
		}
		// If not, calculate distance to end node
		else
			hCost = CalculateH(endNode);
	}
	// Calculate fCost
	if (startNode && endNode)
	{
		fCost = gCost + hCost;
	}

	// Only render costs if it is not the start or end node
	if (startNode && endNode)
		if (startNode->gridPos != gridPos && endNode->gridPos != gridPos)
		{
			fCostText.setString(std::to_string(fCost));
			gCostText.setString(std::to_string(gCost));
			hCostText.setString(std::to_string(hCost));
		}
}

void Node::Render(sf::RenderWindow* window)
{
	window->draw(shape);
	window->draw(gCostText);
	window->draw(hCostText);
	window->draw(fCostText);
}

int Node::CalculateG(Node* startNode)
{
	int startNodeX, startNodeY;
	startNodeX = startNode->gridPos.x;
	startNodeY = startNode->gridPos.y;
	// x distance
	int xDistance = abs(gridPos.x - startNodeX);
	// y distance
	int yDistance = abs(gridPos.y - startNodeY);
	// Work out which distance is larger
	int greaterDistance;
	if (xDistance != yDistance)
		greaterDistance = xDistance > yDistance ? xDistance : yDistance;
	else
		greaterDistance = xDistance;
	// Work out number of diagonals
		int diagonalsNum = greaterDistance - abs(xDistance - yDistance);

	return ((greaterDistance - diagonalsNum) * 10) + (diagonalsNum * 14);
}

int Node::CalculateH(Node* endNode)
{
	int endNodeX, endNodeY;
	endNodeX = endNode->gridPos.x;
	endNodeY = endNode->gridPos.y;
	// x distance
	int xDistance = abs(gridPos.x - endNodeX);
	// y distance
	int yDistance = abs(gridPos.y - endNodeY);
	// Work out which distance is larger
	int greaterDistance;
	if (xDistance != yDistance)
		greaterDistance = xDistance > yDistance ? xDistance : yDistance;
	else
		greaterDistance = xDistance;
	// Work out number of diagonals
	int diagonalsNum = greaterDistance - abs(xDistance - yDistance);

	return (greaterDistance - diagonalsNum) * 10 + (diagonalsNum * 14);
}
