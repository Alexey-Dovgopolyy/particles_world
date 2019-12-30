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

#include <thread>

sf::Time sTimePerFrame = sf::seconds(1.f / 30.f);

void drawThreadFunc()
{
    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
    World* world = ServiceProvider::getWorldService()->getWorld();

    while (window->isOpen())
    {
        window->clear();
        world->draw();
        ServiceProvider::getDataTextService()->draw();
        window->display();
    }
}

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

    sf::Clock updateTimer;

    window->setActive(false);
    std::thread drawThread(drawThreadFunc);

    while (window->isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate >= sTimePerFrame)
        {
            timeSinceLastUpdate -= sTimePerFrame;

            inputService->processInput();
            communication->executeAll();

            int times = world->getUpdateTimes();

            updateTimer.restart();
            for (int i = 0; i < times; i++)
            {
                world->update(sTimePerFrame.asSeconds());
            }
            
            float updateTime = updateTimer.restart().asSeconds();
            ServiceProvider::getDataTextService()->setUpdateTime(updateTime);

            ServiceProvider::getDataTextService()->update();
        }
    }

    drawThread.join();

    ServiceProvider::cleanup();

    return 0;
}