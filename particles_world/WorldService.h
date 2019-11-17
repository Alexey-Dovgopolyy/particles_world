#pragma once

#include "World.h"

class WorldService
{
    friend class ServiceProvider;

public:
    World* getWorld();

private:
    WorldService();
    WorldService(const WorldService& window) = delete;

    bool init();

    static void create();
    static void cleanup();
    static WorldService* getInstance();

private:
    World mWorld;

    static WorldService* sInstance;
};

