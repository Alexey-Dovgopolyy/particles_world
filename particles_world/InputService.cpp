#include "InputService.h"
#include "ServiceProvider.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

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
    }
}
