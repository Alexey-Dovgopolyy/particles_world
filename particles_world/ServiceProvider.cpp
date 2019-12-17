#include "ServiceProvider.h"

#include "WindowService.h"
#include "ConfigService.h"
#include "InputService.h"
#include "WorldService.h"
#include "CommunicationService.h"
#include "DataTextService.h"
#include "PhysicsService.h"

#include <iostream>

void ServiceProvider::create()
{
    CommunicationService::create();
    ConfigService::create();
    WindowService::create();
    InputService::create();
    WorldService::create();
    DataTextService::create();
    PhysicsService::create();
}

void ServiceProvider::cleanup()
{
    ConfigService::cleanup();
    WindowService::cleanup();
    InputService::cleanup();
    WorldService::cleanup();
    CommunicationService::cleanup();
    DataTextService::cleanup();
    PhysicsService::cleanup();
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

    if (!DataTextService::getInstance()->init())
    {
        std::cout << "ERROR: data service" << std::endl;
    }

    if (!PhysicsService::getInstance()->init())
    {
        std::cout << "ERROR: physics service" << std::endl;
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

DataTextService* ServiceProvider::getDataTextService()
{
    return DataTextService::getInstance();
}

PhysicsService* ServiceProvider::getPhysicsService()
{
    return PhysicsService::getInstance();
}
