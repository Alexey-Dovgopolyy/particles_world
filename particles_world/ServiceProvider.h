#pragma once

#include "WindowService.h"
#include "ConfigService.h"
#include "InputService.h"

class ServiceProvider
{
public:
    static void create();
    static void init();
    static void cleanup();

    static WindowService* getWindowService();
    static ConfigService* getConfigService();
    static InputService* getInputService();
};

