#include "WindowProvider.h"

WindowProvider* WindowProvider::sInstance = nullptr;

WindowProvider::WindowProvider()
{

}

void WindowProvider::create()
{
    sInstance = new WindowProvider();
}

void WindowProvider::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

void WindowProvider::init()
{
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Physics");
}

sf::RenderWindow* WindowProvider::getWindow()
{
    return mWindow.get();
}

WindowProvider* WindowProvider::getInstance()
{
    return sInstance;
}
