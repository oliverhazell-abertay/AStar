#include <SFML/Graphics.hpp>
#include "node.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

const int gridWidth = 5;
const int gridHeight = 5;
const float shapeWidth = WINDOW_WIDTH / gridWidth;
const float shapeHeight = WINDOW_HEIGHT / gridHeight;
Node grid[gridWidth][gridHeight];

void InitGrid()
{
    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            // Variables
            grid[i][j].SetWorldPos(sf::Vector2f(shapeWidth * i, shapeHeight * j));
            grid[i][j].SetSize(sf::Vector2f(shapeWidth, shapeHeight));
            grid[i][j].shape.setFillColor(sf::Color::White);
            grid[i][j].shape.setOutlineColor(sf::Color::Black);
            grid[i][j].shape.setOutlineThickness(5.0f);
            // Neighbours
            //North
            if (j > 0)
                grid[i][j].northAdj = &grid[i][j - 1];
            // South
            if (j < gridHeight - 1)
                grid[i][j].southAdj = &grid[i][j + 1];
            // East
            if (i < gridWidth - 1)
                grid[i][j].eastAdj = &grid[i + 1][j];
            // West
            if (i > 0)
                grid[i][j].westAdj = &grid[i - 1][j];

        }
    }
}

void FindGridSquareCollision(sf::RenderWindow* window, sf::Color color)
{
    auto mouse_pos = sf::Mouse::getPosition(*window); // Mouse position relative to the window
    auto translated_pos = window->mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates

    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            if (grid[i][j].shape.getGlobalBounds().contains(translated_pos))
            {
                grid[i][j].shape.setFillColor(color);
                // Fill neighbours
                if(grid[i][j].northAdj)
                    grid[i][j].northAdj->shape.setFillColor(color);
                if (grid[i][j].southAdj)
                    grid[i][j].southAdj->shape.setFillColor(color);
                if (grid[i][j].eastAdj)
                    grid[i][j].eastAdj->shape.setFillColor(color);
                if (grid[i][j].westAdj)
                    grid[i][j].westAdj->shape.setFillColor(color);
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "A* Pathfinding");
    window.setFramerateLimit(60);

    // Init grid
    InitGrid();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // ESC - Close window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            // Left Mouse button - Change grid square to green
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                FindGridSquareCollision(&window, sf::Color::Green);
            }
            // Left Mouse button - Change grid square to white
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                FindGridSquareCollision(&window, sf::Color::White);
            }
        }
        // Render
        window.clear();
            // Draw grid
            for (int i = 0; i < gridWidth; i++)
            {
                for (int j = 0; j < gridHeight; j++)
                {
                    window.draw(grid[i][j].shape);
                }
            }
        window.display();
        // Update nodes
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                grid[i][j].Update();
            }
        }
    }

    return 0;
}