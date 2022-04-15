#include "application.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>


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
        window->setKeyRepeatEnabled(false);
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            // ESC - Close window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window->close();
            // Space bar - Find path between nodes
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    if (startNode && endNode)
                    {
                        std::cout << "Space bar pressed\n";
                        FindPath();
                    }
                }
            }
            // R - Reset grid
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                InitGrid();
            // Left Mouse button - Assign start node
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                FindGridSquareCollision(window, 0);
            }
            // Right Mouse button - Assign end node
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                FindGridSquareCollision(window, 1);
            }
            // Middle Mouse button - Assign obstacle
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                FindGridSquareCollision(window, 2);
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

void Application::FindGridSquareCollision(sf::RenderWindow* window, int nodeType)
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
                if (nodeType == 0)
                    startNode = &grid[i][j];
                // If right clicked, make end node
                else if (nodeType == 1)
                    endNode = &grid[i][j];
                // If middle clicked, make obstacle
                else if (nodeType == 2)
                    grid[i][j].obstacle = true;
                // If none of the above, throw error
                else if (nodeType > 2 || nodeType < 0)
                    std::cout << "Node type is outside bounds!";
            }
        }
    }
}

// Helper function to find lowest f cost
bool Application::CompareF(Node* n1, Node* n2)
{
    return n1->GetFCost() < n2->GetFCost();
}

void Application::FindPath()
{
    // Add start node to open
    openNodes.push_back(startNode);
    Node* currentNode = *openNodes.begin();
    bool pathFound = false;
    // Loop to find path
    while (!pathFound)
    {
        int lowestF = 1000000;
        // Find node with lowest f cost
        for (auto& it : openNodes)
        {
            // If f cost is lowest so far
            if (it->GetFCost() < lowestF)
            {
                currentNode = it;
                lowestF = it->GetFCost();
            }
            // If f cost is equal, check if h cost is lower
            else if (it->GetFCost() == lowestF && it->GetHCost() < currentNode->GetHCost())
            {
                currentNode = it;
                lowestF = it->GetFCost();
            }
        }
        // Remove lowest f node from open, add to closed
        if (std::find(openNodes.begin(), openNodes.end(), currentNode) != openNodes.end())
           openNodes.remove(currentNode);
        if (std::find(closedNodes.begin(), closedNodes.end(), currentNode) == closedNodes.end())
         closedNodes.push_back(currentNode);
        
        // If current node is the target node, path found
        if (currentNode == endNode)
            pathFound = true;
        // If not, carry on searching using lowest f node
        else
        {
            // Calculate distance to each neighbour node
            // Orthogonal neighbours
            for (auto& it : currentNode->neighboursOrth)
            {
                if (it == endNode)
                {
                    it->parent = currentNode;
                    pathFound = true;
                }
                // If neighbour isn't an obstacle or in closed
                else if (!it->obstacle && std::find(closedNodes.begin(), closedNodes.end(), it) == closedNodes.end())
                {
                    // If neighbour isn't in open or new path to neighbour is shorter
                    if (std::find(openNodes.begin(), openNodes.end(), it) == openNodes.end() || (it->GetFCost() + 10) < it->GetFCost())
                    {
                        it->CalculateFGH(startNode, endNode);
                        it->parent = currentNode;
                        // If neighbour isn't in open, add it
                        if (std::find(openNodes.begin(), openNodes.end(), it) == openNodes.end())
                            openNodes.push_back(it);
                    }
                }
            }
            // Diagonal neighbours
            for (auto& it : currentNode->neighboursDiag)
            {
                // If neighbour isn't an obstacle or in closed
                if (!it->obstacle && std::find(closedNodes.begin(), closedNodes.end(), it) == closedNodes.end())
                {
                    // If neighbour isn't in open or new path to neighbour is shorter
                    if (std::find(openNodes.begin(), openNodes.end(), it) == openNodes.end() || (it->GetFCost() + 14) < it->GetFCost())
                    {
                        it->CalculateFGH(startNode, endNode);
                        it->parent = currentNode;
                        // If neighbour isn't in open, add it
                        if (std::find(openNodes.begin(), openNodes.end(), it) == openNodes.end())
                            openNodes.push_back(it);
                    }
                }
            }
        }
        Render();
    }

    // Colour path
    currentNode = endNode;
    while (currentNode != startNode)
    {
        currentNode->shape.setFillColor(sf::Color::Blue);
        currentNode = currentNode->parent;
    }
}