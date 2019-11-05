#include "ServiceProvider.h"

#include <iostream>

void ServiceProvider::create()
{
    ConfigService::create();
    WindowService::create();
}

void ServiceProvider::cleanup()
{
    ConfigService::cleanup();
    WindowService::cleanup();
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
}

WindowService* ServiceProvider::getWindowService()
{
    return WindowService::getInstance();
}

ConfigService* ServiceProvider::getConfigService()
{
    return ConfigService::getInstance();
}
