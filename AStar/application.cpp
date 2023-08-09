#include "application.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#define ORTH_COST 10
#define DIAG_COST 14

void Application::Init(sf::RenderWindow* wind)
{
    window = wind;
    myFont.loadFromFile("C:\\Users\\olive\\Documents\\Portfolio\\AStar\\AStar\\Fonts\\OpenSans-Light.ttf");
    // Init grid
    InitGrid();
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
    while (window->isOpen())
    {
        // Input
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
                        auto start = chrono::system_clock::now();
                        if (FindPath())
                        {
                            cout << "Path found!\n";
                        }
                        else
                        {
                            cout << "Returned false!\n";
                        }
                            
                        auto end = chrono::system_clock::now();
                        auto elasped = chrono::duration_cast<chrono::milliseconds>(end - start);
                        cout << elasped.count() << '\n';
                        ColourPath();
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
            // North
            if (j > 0)
                grid[i][j].neighboursOrth.push_back(sf::Vector2i(i, j - 1));
            // South
            if (j < gridHeight - 1)
                grid[i][j].neighboursOrth.push_back(sf::Vector2i(i, j + 1));
            // East
            if (i < gridWidth - 1)
                grid[i][j].neighboursOrth.push_back(sf::Vector2i(i + 1, j));
            // West
            if (i > 0)
                grid[i][j].neighboursOrth.push_back(sf::Vector2i(i - 1, j));
            // North west
            if(i > 0 && j > 0)
                grid[i][j].neighboursDiag.push_back(sf::Vector2i(i - 1, j - 1));
            // North east
            if (i < gridWidth - 1 && j > 0)
                grid[i][j].neighboursDiag.push_back(sf::Vector2i(i + 1, j - 1));
            // South west
            if (i > 0 && j < gridHeight - 1)
                grid[i][j].neighboursDiag.push_back(sf::Vector2i(i - 1, j + 1));
            // South east
            if (i < gridWidth - 1 && j < gridHeight - 1)
                grid[i][j].neighboursDiag.push_back(sf::Vector2i(i + 1, j + 1));
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
                    cout << "Node type is outside bounds!";
            }
        }
    }
}

bool Application::FindPath()
{
    // Add start node to open
    startNode->CalculateF();
    openNodes.insert(make_pair(startNode->GetFCost(), make_pair(startNode->gridPos.x, startNode->gridPos.y)));
    Node* currentNode;
    bool pathFound = false;
    // Init closed list 
    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            closedNodes[gridWidth][gridHeight] = false;
        }
    }
    // Loop to find path
    while (!pathFound)
    {
        // If openNodes are empty, no path is possible
        if (openNodes.empty())
        {
            cout << "No path possible!\n";
            return pathFound;
        }
        // Take node with lowest f cost
        pPair p = *openNodes.begin();
        int i = p.second.first;
        int j = p.second.second;
        currentNode = &grid[i][j];
        // Remove from open, add to closed
        openNodes.erase(openNodes.begin());
        closedNodes[i][j] = true;
        // If current node is the target node, path found
        if (currentNode == endNode)
        {
            pathFound = true;
        }
        // If not, add neighbours of current node to open list
        else
        {
            // Calculate distance to each neighbour node
            // Orthogonal neighbours
            for (auto& it : currentNode->neighboursOrth)
            {
                Node* examNode = &grid[it.x][it.y];
                // If neighbour isn't an obstacle or in closed list
                if (!examNode->obstacle && !closedNodes[it.x][it.y])
                {
                    // If neighbour isn't in open, add to open and make parent
                    if (examNode->GetFCost() == -1)
                    {
                        // Calculate values
                        examNode->SetGCost(currentNode->GetGCost() + ORTH_COST);
                        examNode->CalculateH(endNode);
                        examNode->CalculateF();
                        // Add parent to currentnode
                        examNode->parentX = currentNode->gridPos.x;
                        examNode->parentY = currentNode->gridPos.y;
                        // Add to open list
                        pPair tempPair = make_pair(examNode->GetFCost(), make_pair(examNode->gridPos.x, examNode->gridPos.y));
                        openNodes.insert(tempPair);
                    }
                    // If neighbour is already on open list, check if current node is better path
                    else
                    {
                        // If path to current node is better than original g cost, set parent to current
                        if (examNode->GetGCost() > currentNode->GetGCost() + ORTH_COST)
                        {
                            examNode->SetGCost(currentNode->GetGCost() + ORTH_COST);
                            examNode->CalculateF();
                            examNode->parentX = currentNode->gridPos.x;
                            examNode->parentY = currentNode->gridPos.y;
                        }
                    }
                }
            }
            // Diagonal neighbours
            for (auto it : currentNode->neighboursDiag)
            {
                Node* examNode = &grid[it.x][it.y];
                // If neighbour isn't an obstacle or in closed list
                if (!examNode->obstacle && !closedNodes[it.x][it.y])
                {
                    // If neighbour isn't in open, add to open and make parent
                    if (examNode->GetFCost() == -1)
                    {
                        // Calculate values
                        examNode->SetGCost(currentNode->GetGCost() + DIAG_COST);
                        examNode->CalculateH(endNode);
                        examNode->CalculateF();
                        // Add parent to currentnode
                        examNode->parentX = currentNode->gridPos.x;
                        examNode->parentY = currentNode->gridPos.y;
                        // Add to open list
                        pPair tempPair = make_pair(examNode->GetFCost(), make_pair(examNode->gridPos.x, examNode->gridPos.y));
                        openNodes.insert(tempPair);
                    }
                    // If neighbour is already on open list, check if current node is better path
                    else
                    {
                        // If path to current node is better than original g cost, set parent to current
                        if (examNode->GetGCost() > currentNode->GetGCost() + DIAG_COST)
                        {
                            examNode->SetGCost(currentNode->GetGCost() + DIAG_COST);
                            examNode->CalculateF();
                            examNode->parentX = currentNode->gridPos.x;
                            examNode->parentY = currentNode->gridPos.y;
                        }
                    }
                }
            }
        }
    }
    // If it made it out the loop, path found
    return true;
}

void Application::ColourPath()
{
    // Colour path
    Node* currentNode = endNode;
    while (currentNode != startNode)
    {
        currentNode->shape.setFillColor(sf::Color::Blue);
        currentNode = &grid[currentNode->parentX][currentNode->parentY];
    }
}