#include "application.h"

void Application::Init(sf::RenderWindow* wind)
{
    window = wind;
    myFont.loadFromFile("C:\\Users\\olive\\Documents\\Portfolio\\AStar\\AStar\\Fonts\\OpenSans-Light.ttf");
}

void Application::CleanUp()
{
    delete window;
    window = NULL;
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
            // Left Mouse button - Change grid square to green
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                FindGridSquareCollision(window, sf::Color::Green);
            }
            // Left Mouse button - Change grid square to white
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                FindGridSquareCollision(window, sf::Color::White);
            }
        }
        // Update nodes
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                grid[i][j].Update();
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
            grid[i][j].Init(sf::Vector2f(shapeWidth * i, shapeHeight * j), sf::Vector2f(shapeWidth, shapeHeight), &myFont);
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
            //North west
            if(grid[i][j].northAdj && grid[i][j].westAdj)
                grid[i][j].northWestAdj = &grid[i - 1][j - 1];
            //North east
            if (grid[i][j].northAdj && grid[i][j].eastAdj)
                grid[i][j].northEastAdj = &grid[i + 1][j - 1];
            //South west
            if (grid[i][j].southAdj && grid[i][j].westAdj)
                grid[i][j].southWestAdj = &grid[i - 1][j + 1];
            //South east
            if (grid[i][j].southAdj && grid[i][j].eastAdj)
                grid[i][j].southEastAdj = &grid[i + 1][j + 1];

        }
    }
}

void Application::FindGridSquareCollision(sf::RenderWindow* window, sf::Color color)
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
            }
        }
    }
}