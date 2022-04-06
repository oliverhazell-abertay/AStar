#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "A* Pathfinding");
    window.setFramerateLimit(60);

    sf::RectangleShape rect;
    sf::Vector2f rectPosition(590.f, 310.f);
    rect.setPosition(rectPosition);
    rect.setSize(sf::Vector2f(100.f, 100.f));

    float xVelocity = 3.f;
    float yVelocity = 3.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //ESC - Close window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }
        //collision detection
        if (rectPosition.x < 0 || rectPosition.x > 1280 - rect.getSize().x)
            xVelocity *= -1;
        if (rectPosition.y < 0 || rectPosition.y > 720 - rect.getSize().y)
            yVelocity *= -1;
        //physics
        rectPosition.x += xVelocity;
        rectPosition.y += yVelocity;
        rect.setPosition(rectPosition);
        //render
        window.clear();
        window.draw(rect);
        window.display();
    }



    return 0;
}