#include "SFML/Graphics.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "ServiceProvider.h"
#include "WindowService.h"
#include "WorldService.h"
#include "InputService.h"
#include "CommunicationService.h"
#include "DataTextService.h"
#include "World.h"

sf::Time sTimePerFrame = sf::seconds(1.f / 30.f);

int main()
{
    ServiceProvider::create();
    ServiceProvider::init();

    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
    
    InputService* inputService = ServiceProvider::getInputService();
    CommunicationService* communication = ServiceProvider::getCommunicationService();

    World* world = ServiceProvider::getWorldService()->getWorld();

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero; 

    while (window->isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate > sTimePerFrame)
        {
            timeSinceLastUpdate -= sTimePerFrame;

            inputService->processInput();
            communication->executeAll();

            world->update(sTimePerFrame.asSeconds());
        }

        window->clear();

        world->draw();

        ServiceProvider::getDataTextService()->update(dt);
        ServiceProvider::getDataTextService()->draw();

        window->display();
    }

    ServiceProvider::cleanup();

    return 0;
}