#pragma once

#include "common_types.h"

class ConfigService
{
    friend class ServiceProvider;

public:
    int getWinSizeX() const;
    int getWinSizeY() const;

    float getAttractionRadius() const;
    float getAttractionCoef() const;

    float getRepelRadius() const;
    float getRepelCoef() const;

    float getParticleRadius() const;
    float getParticleDrawRadius() const;

    float getGravitation() const;
    float getParticleSpawnPeriod() const;

    float getECoef() const;
    int getRepelPow() const;
    int getAttractPow() const;

    float getCollideRadius() const;
    float getNoForceDistCoef() const;

    const DebugConfig& getDebugConfig() const;

private:
    ConfigService();
    ConfigService(const ConfigService& config) = delete;

    bool init();

    static void create();
    static void cleanup();
    static ConfigService* getInstance();

private:
    static ConfigService* sInstance;

    int mWindowSizeX = 800;
    int mWindowSizeY = 600;

    float mAttractionRadius = 100.f;
    float mAttractionCoef = 0.1f;

    float mRepelRadius = 4.f;
    float mRepelCoef = 1.f;

    float mParticleRadius = 2.f;
    float mParticleDrawRadius = 2.f;

    float mGravitation = 1.f;
    float mParticleSpawnPeriod = 0.1f;

    float mECoef = 1.f;
    int mRepelPow = 12;
    int mAttractPow = 6;

    float mCollideRadius = 2.f;
    float mNoForceDistCoef = 2.5f;

    DebugConfig mDebugConfig;
};

