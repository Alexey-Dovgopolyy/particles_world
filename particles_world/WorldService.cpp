#include "WorldService.h"

WorldService* WorldService::sInstance = nullptr;

WorldService::WorldService()
{

}

bool WorldService::init()
{
    bool result = mWorld.init();
    return result;
}

void WorldService::create()
{
    sInstance = new WorldService();
}

void WorldService::cleanup()
{
    World* world = getInstance()->getWorld();
    world->cleanup();

    delete sInstance;
    sInstance = nullptr;
}

WorldService* WorldService::getInstance()
{
    return sInstance;
}

World* WorldService::getWorld()
{
    return &mWorld;
}
