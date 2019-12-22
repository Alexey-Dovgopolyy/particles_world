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

    mParticleRadius = document["particle_radius"].GetFloat();
    mParticleDrawRadius = document["particle_draw_radius"].GetFloat();

    mGravitation = document["gravitation_force"].GetFloat();
    mParticleSpawnPeriod = document["particle_spawn_period"].GetFloat();

    mECoef = document["e_coef"].GetFloat();
    mRepelPow = document["repel_power"].GetInt();
    mAttractPow = document["attract_power"].GetInt();

    mCollideRadius = document["collide_dist"].GetFloat();
    mNoForceDistCoef = document["no_force_dist_coef"].GetFloat();

    mSpeedIncStep = document["speed_inc_step"].GetFloat();
    mMaxInitialSpeed = document["max_init_speed"].GetFloat();

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

float ConfigService::getParticleRadius() const
{
    return mParticleRadius;
}

float ConfigService::getParticleDrawRadius() const
{
    return mParticleDrawRadius;
}

float ConfigService::getGravitation() const
{
    return mGravitation;
}

float ConfigService::getParticleSpawnPeriod() const
{
    return mParticleSpawnPeriod;
}

float ConfigService::getECoef() const
{
    return mECoef;
}

int ConfigService::getRepelPow() const
{
    return mRepelPow;
}

int ConfigService::getAttractPow() const
{
    return mAttractPow;
}

float ConfigService::getCollideRadius() const
{
    return mCollideRadius;
}

float ConfigService::getNoForceDistCoef() const
{
    return mNoForceDistCoef;
}

float ConfigService::getSpeedIncStep() const
{
    return mSpeedIncStep;
}

float ConfigService::getMaxInitialSpeed() const
{
    return mMaxInitialSpeed;
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
