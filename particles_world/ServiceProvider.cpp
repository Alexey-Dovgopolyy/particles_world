#include "ServiceProvider.h"
#include "WindowProvider.h"


void ServiceProvider::create()
{
    WindowProvider::create();
}

void ServiceProvider::cleanup()
{
    WindowProvider::cleanup();
}

void ServiceProvider::init()
{
    WindowProvider::getInstance()->init();
}

WindowProvider* ServiceProvider::getWindowProvider()
{
    return WindowProvider::getInstance();
}
