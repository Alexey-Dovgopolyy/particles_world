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
            window->close();
        }

        if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            std::cout << "mouse moved" << std::endl;
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            event.mouseWheelScroll.y;
        }
    }
}
