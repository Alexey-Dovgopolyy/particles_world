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

    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
    
    InputService* inputService = ServiceProvider::getInputService();

    World world;
    world.init();

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window->isOpen())
    {
        inputService->processInput();

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

    world.cleanup();

    ServiceProvider::cleanup();

    return 0;
}