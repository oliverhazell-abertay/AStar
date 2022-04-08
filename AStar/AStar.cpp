#include <SFML/Graphics.hpp>
#include "application.h"
#include "node.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "A* Pathfinding");
    window.setFramerateLimit(60);

    Application application;
    application.Init(&window);

    while (window.isOpen())
    {
        application.Update();
    }

    return 0;
}