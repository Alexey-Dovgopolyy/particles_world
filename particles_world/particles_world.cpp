#include "SFML/Graphics.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "ServiceProvider.h"
#include "World.h"

sf::Time sTimePerFrame = sf::seconds(1.f / 90.f);

int main()
{
    ServiceProvider::create();
    ServiceProvider::init();

    sf::RenderWindow* window = ServiceProvider::getWindowProvider()->getWindow();

    World world;
    world.init();

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window->isOpen())
    {
        //bool needCloseWindow = world.processInput();
//         if (needCloseWindow)
//         {
//             break;
//         }

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate > sTimePerFrame)
        {
            timeSinceLastUpdate -= sTimePerFrame;

            world.update(sTimePerFrame.asSeconds());
        }

        window->clear();
        world.draw();
        window->display();
    }

    ServiceProvider::cleanup();

    return 0;
}