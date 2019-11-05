#include "WindowService.h"

WindowService* WindowService::sInstance = nullptr;

WindowService::WindowService()
{

}

void WindowService::create()
{
    sInstance = new WindowService();
}

void WindowService::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

bool WindowService::init()
{
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Physics");

    bool result = (mWindow != nullptr);
    return result;
}

sf::RenderWindow* WindowService::getWindow()
{
    return mWindow.get();
}

WindowService* WindowService::getInstance()
{
    return sInstance;
}
