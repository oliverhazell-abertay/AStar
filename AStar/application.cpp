#include "application.h"
#include <iostream>
#include <algorithm>

void Application::Init(sf::RenderWindow* wind)
{
    window = wind;
    myFont.loadFromFile("C:\\Users\\olive\\Documents\\Portfolio\\AStar\\AStar\\Fonts\\OpenSans-Light.ttf");
}

void Application::CleanUp()
{
    delete window;
    window = NULL;
    delete startNode;
    startNode = NULL;

    delete endNode;
    endNode = NULL;
}

int Application::Update()
{
    // Init grid
    InitGrid();

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            // ESC - Close window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window->close();
            // Space bar - Find path between nodes
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (!findingPath && startNode && endNode)
                {
                    findingPath = true;
                        FindPath();
                    findingPath = false;
                }
            }
            // Left Mouse button - Change grid square to green
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                FindGridSquareCollision(window, true);
            }
            // Right Mouse button - Change grid square to white
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                FindGridSquareCollision(window, false);
            }
        }
        // Update nodes
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                grid[i][j].Update(startNode, endNode);
            }
        }
        // Render
        Render();
    }

    return 0;
}

void Application::Render()
{
    window->clear();
    // Draw grid
    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            grid[i][j].Render(window);
        }
    }
    window->display();
}

void Application::InitGrid()
{
    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            // Variables
            grid[i][j].Init(sf::Vector2f(shapeWidth * i, shapeHeight * j), sf::Vector2f(shapeWidth, shapeHeight), sf::Vector2i(i, j), &myFont);
            grid[i][j].shape.setFillColor(sf::Color::White);
            grid[i][j].shape.setOutlineColor(sf::Color::Black);
            grid[i][j].shape.setOutlineThickness(5.0f);
            // Neighbours
            //North
            if (j > 0)
                grid[i][j].neighboursOrth.push_back(&grid[i][j - 1]);
            // South
            if (j < gridHeight - 1)
                grid[i][j].neighboursOrth.push_back(&grid[i][j + 1]);
            // East
            if (i < gridWidth - 1)
                grid[i][j].neighboursOrth.push_back(&grid[i + 1][j]);
            // West
            if (i > 0)
                grid[i][j].neighboursOrth.push_back(&grid[i - 1][j]);
            //North west
            if(i > 0 && j > 0)
                grid[i][j].neighboursDiag.push_back(&grid[i - 1][j - 1]);
            //North east
            if (i < gridWidth - 1 && j > 0)
                grid[i][j].neighboursDiag.push_back(&grid[i + 1][j - 1]);
            //South west
            if (i > 0 && j < gridHeight - 1)
                grid[i][j].neighboursDiag.push_back(&grid[i - 1][j + 1]);
            //South east
            if (i < gridWidth - 1 && j < gridHeight - 1)
                grid[i][j].neighboursDiag.push_back(&grid[i + 1][j + 1]);

        }
    }
}

void Application::FindGridSquareCollision(sf::RenderWindow* window, bool leftClick)
{
    auto mouse_pos = sf::Mouse::getPosition(*window); // Mouse position relative to the window
    auto translated_pos = window->mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates

    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            if (grid[i][j].shape.getGlobalBounds().contains(translated_pos))
            {
                // If left clicked, make start node
                if (leftClick)
                    startNode = &grid[i][j];
                // If not then it was right clicked, make end node
                else
                    endNode = &grid[i][j];
            }
        }
    }
}

void Application::FindPath()
{
    Node* currentNode;
    currentNode = startNode;
    while (currentNode != endNode)
    {
        // Calculate FGH for neighbours of current node
        for (auto& it : currentNode->neighboursOrth)
        {
            // If not already visited
            if (std::find(visitedNodes.begin(), visitedNodes.end(), it) == visitedNodes.end())
            {
                it->CalculateFGH(startNode, endNode);
                // Add neighbour to visited nodes if not already visited
                visitedNodes.push_back(it);
            }
        }
        for (auto& it : currentNode->neighboursDiag)
        {
            // If not already visited
            if (std::find(visitedNodes.begin(), visitedNodes.end(), it) == visitedNodes.end())
            {
                it->CalculateFGH(startNode, endNode);
                // Add neighbour to visited nodes
                if (std::find(visitedNodes.begin(), visitedNodes.end(), it) == visitedNodes.end())
                    visitedNodes.push_back(it);
            }
        }

        // Add current node to examined
        examinedNodes.push_back(currentNode);

        // Find node with lowest f value in grid that hasn't been examined
        int lowestF = 1000000;
        for (auto& it : visitedNodes)
        {
            // If not already visited
            if (std::find(examinedNodes.begin(), examinedNodes.end(), it) == examinedNodes.end())
            {
                if (it->GetFCost() != 0 && it->GetFCost() < lowestF)
                {
                    currentNode = it;
                    lowestF = currentNode->GetFCost();
                }
            }
        }
    }

    // End node found, start at the end node and find shortest route back
    currentNode = endNode;
    Node* nextNode = currentNode;
    while (currentNode != startNode)
    {
        // Find lowest f value of neighbour of current node
        int lowestF = 100000;
        for (auto& it : currentNode->neighboursOrth)
        {
            // Check if neighbour is start node
            if (it == startNode)
                currentNode = it;
            // Check neighbour isn't already in path
            else if (std::find(path.begin(), path.end(), it) == path.end())
            {
                if (it->GetFCost() != 0 && it->GetFCost() < lowestF)
                {
                    lowestF = it->GetFCost();
                    nextNode = it;
                }
            }
        }
        for (auto& it : currentNode->neighboursDiag)
        {
            // Check if neighbour is start node
            if (it == startNode)
                currentNode = it;
            // Check neighbour isn't already in path
            else if (std::find(path.begin(), path.end(), it) == path.end())
            {
                if (it->GetFCost() != 0 && it->GetFCost() < lowestF)
                {
                    lowestF = it->GetFCost();
                    nextNode = it;
                }
            }
        }
        // If we're not at the start node, add node to path
        if (currentNode != startNode)
        {
            path.push_back(currentNode);
            // Prepare for next loop
            currentNode = nextNode;
        }
    }
    // Colour path
    for (auto& it : path)
    {
        it->shape.setFillColor(sf::Color::Yellow);
    }
}