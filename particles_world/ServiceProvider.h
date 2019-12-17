#pragma once

class WindowService;
class ConfigService;
class InputService;
class WorldService;
class CommunicationService;
class DataTextService;
class PhysicsService;

class ServiceProvider
{
public:
    static void create();
    static void init();
    static void cleanup();

    static WindowService* getWindowService();
    static ConfigService* getConfigService();
    static InputService* getInputService();
    static WorldService* getWorldService();
    static CommunicationService* getCommunicationService();
    static DataTextService* getDataTextService();
    static PhysicsService* getPhysicsService();
};

