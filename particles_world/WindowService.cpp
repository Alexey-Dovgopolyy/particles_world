#include "WindowService.h"
#include "ServiceProvider.h"
#include "CommunicationService.h"
#include "ConfigService.h"

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
    ServiceProvider::getCommunicationService()->addListener(MessageType::closeWindow, sInstance);

    ConfigService* config = ServiceProvider::getConfigService();
    int windowWidth = config->getWinSizeX();
    int windowHeight = config->getWinSizeY();

    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowWidth, windowHeight), "Physics");

    bool result = (mWindow != nullptr);
    return result;
}

sf::RenderWindow* WindowService::getWindow()
{
    return mWindow.get();
}

void WindowService::handleMessage(MessageType messageType, Message* message)
{
    if (messageType == MessageType::closeWindow)
    {
        mWindow->close();
    }
}

WindowService* WindowService::getInstance()
{
    return sInstance;
}
