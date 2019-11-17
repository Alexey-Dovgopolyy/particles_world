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

    mDebugConfig.debugP1PosX = document["debug_p1_pos_x"].GetFloat();
    mDebugConfig.debugP1PosY = document["debug_p1_pos_y"].GetFloat();
    mDebugConfig.debugP2PosX = document["debug_p2_pos_x"].GetFloat();
    mDebugConfig.debugP2PosY = document["debug_p2_pos_y"].GetFloat();

    mDebugConfig.debugP1DirX = document["debug_p1_dir_x"].GetFloat();
    mDebugConfig.debugP1DirY = document["debug_p1_dir_y"].GetFloat();
    mDebugConfig.debugP2DirX = document["debug_p2_dir_x"].GetFloat();
    mDebugConfig.debugP2DirY = document["debug_p2_dir_y"].GetFloat();

    mDebugConfig.debugP1Speed = document["debug_p1_speed"].GetFloat();
    mDebugConfig.debugP2Speed = document["debug_p2_speed"].GetFloat();

    mDebugConfig.debugSpawnPosX = document["debug_spawn_zone_pos_x"].GetFloat();
    mDebugConfig.debugSpawnPosY = document["debug_spawn_zone_pos_y"].GetFloat();

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

const DebugConfig& ConfigService::getDebugConfig() const
{
    return mDebugConfig;
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
