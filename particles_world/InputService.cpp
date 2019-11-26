#include "InputService.h"
#include "ServiceProvider.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

InputService::InputService()
{

}

bool InputService::init()
{
    return false;
}

void InputService::create()
{
}

void InputService::cleanup()
{
}

InputService* InputService::getInstance()
{
    return nullptr;
}

void InputService::processInput()
{
    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();

    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            ServiceProvider::getCommunicationService()->queueMessage(MessageType::closeWindow, nullptr);
        }

        if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            ServiceProvider::getCommunicationService()->queueMessage(MessageType::mouseMoved, new MessageMouseMove(mousePos));
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            float delta = event.mouseWheelScroll.delta;
            ServiceProvider::getCommunicationService()->queueMessage(MessageType::mouseWheelMoved, new MessageMouseWheelMove(delta));
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        ServiceProvider::getCommunicationService()->queueMessage(MessageType::spawnParticle, nullptr);
    }
}
