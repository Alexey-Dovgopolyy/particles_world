#pragma once
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
};

