#include "InputService.h"
#include "ServiceProvider.h"
#include "CommunicationService.h"
#include "WindowService.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

InputService* InputService::sInstance = nullptr;

InputService::InputService()
{

}

bool InputService::init()
{
    return true;
}

void InputService::create()
{
    sInstance = new InputService();
}

void InputService::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

InputService* InputService::getInstance()
{
    return sInstance;
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

        if (event.type == sf::Event::GainedFocus)
        {
            mIsMouseInWindow = true;
        }

        if (event.type == sf::Event::LostFocus)
        {
            mIsMouseInWindow = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
        {
            ServiceProvider::getCommunicationService()->queueMessage(MessageType::incInitialSpeed, nullptr);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        {
            ServiceProvider::getCommunicationService()->queueMessage(MessageType::decInitialSpeed, nullptr);
        }

        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Key::Q)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::incAllSpeed, nullptr);
            }
            else if (event.key.code == sf::Keyboard::Key::W)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::decAllSpeed, nullptr);
            }
            else if (event.key.code == sf::Keyboard::Key::E)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::allFreeze, nullptr);
            }
            else if (event.key.code == sf::Keyboard::Key::R)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::switchQuadTree, nullptr);
            }
            else if (event.key.code == sf::Keyboard::Key::A)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::decTime, nullptr);
            }
            else if (event.key.code == sf::Keyboard::Key::S)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::incTime, nullptr);
            }
            else if (event.key.code == sf::Keyboard::Key::D)
            {
                ServiceProvider::getCommunicationService()->queueMessage(MessageType::switchCreateHeat, nullptr);
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mIsMouseInWindow == true)
    {
        ServiceProvider::getCommunicationService()->queueMessage(MessageType::spawnParticle, nullptr);
    }
}
