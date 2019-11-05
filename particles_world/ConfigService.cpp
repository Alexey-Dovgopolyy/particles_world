#include "ConfigService.h"

ConfigService* ConfigService::sInstance = nullptr;

ConfigService::ConfigService()
{

}

bool ConfigService::init()
{
    // load config
    return true;
}

void ConfigService::create()
{
    sInstance = new ConfigService();
}

void ConfigService::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

ConfigService* ConfigService::getInstance()
{
    return sInstance;
}
