#include "ServiceProvider.h"

#include <iostream>

void ServiceProvider::create()
{
    CommunicationService::create();
    ConfigService::create();
    WindowService::create();
    InputService::create();
    WorldService::create();
}

void ServiceProvider::cleanup()
{
    ConfigService::cleanup();
    WindowService::cleanup();
    InputService::cleanup();
    WorldService::cleanup();
    CommunicationService::cleanup();
}

void ServiceProvider::init()
{
    if (!ConfigService::getInstance()->init())
    {
        std::cout << "ERROR: config init" << std::endl;
    }

    if (!WindowService::getInstance()->init())
    {
        std::cout << "ERROR: window init" << std::endl;
    }

    if (!InputService::getInstance()->init())
    {
        std::cout << "ERROR: input init" << std::endl;
    }

    if (!WorldService::getInstance()->init())
    {
        std::cout << "ERROR: world init" << std::endl;
    }

    if (!CommunicationService::getInstance()->init())
    {
        std::cout << "ERROR: communication service" << std::endl;
    }
}

WindowService* ServiceProvider::getWindowService()
{
    return WindowService::getInstance();
}

ConfigService* ServiceProvider::getConfigService()
{
    return ConfigService::getInstance();
}

InputService* ServiceProvider::getInputService()
{
    return InputService::getInstance();
}

WorldService* ServiceProvider::getWorldService()
{
    return WorldService::getInstance();
}

CommunicationService* ServiceProvider::getCommunicationService()
{
    return CommunicationService::getInstance();
}
