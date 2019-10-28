#pragma once

#include "WindowProvider.h"

class ServiceProvider
{
public:
    static void create();
    static void init();
    static void cleanup();

    static WindowProvider* getWindowProvider();
};

