#include "ConfigService.h"
#include "rapidjson/document.h"
#include <fstream>
#include <iostream>
#include <sstream>

ConfigService* ConfigService::sInstance = nullptr;

ConfigService::ConfigService()
{

}

bool ConfigService::init()
{
    std::ifstream configFile("config.json");
    if (!configFile)
    {
        return false;
    }

    std::stringstream buffer;
    buffer << configFile.rdbuf();
    std::string dataStr(buffer.str());

    configFile.close();

    rapidjson::Document document;
    document.Parse(dataStr.c_str());

    if (!document.IsObject())
    {
        return false;
    }

    mWindowSizeX = document["win_size_x"].GetInt();
    mWindowSizeY = document["win_size_y"].GetInt();

    mAttractionRadius = document["attraction_radius"].GetFloat();
    mAttractionCoef = document["attraction_coef"].GetFloat();

    mRepelRadius = document["repel_radius"].GetFloat();
    mRepelCoef = document["repel_coef"].GetFloat();

    return true;
}

int ConfigService::getWinSizeX() const
{
    return mWindowSizeX;
}

int ConfigService::getWinSizeY() const
{
    return mWindowSizeY;
}

float ConfigService::getAttractionRadius() const
{
    return mAttractionRadius;
}

float ConfigService::getAttractionCoef() const
{
    return mAttractionCoef;
}

float ConfigService::getRepelRadius() const
{
    return mRepelRadius;
}

float ConfigService::getRepelCoef() const
{
    return mRepelCoef;
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
