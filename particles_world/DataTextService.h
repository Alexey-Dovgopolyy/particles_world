#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

class DataTextService
{
    friend class ServiceProvider;

public:
    void update(/*float dt*/);
    void draw();

    void setFps(int fps);
    void setUpdateTime(float time);

private:
    DataTextService();
    DataTextService(const DataTextService& window) = delete;

    bool init();

    static void create();
    static void cleanup();
    static DataTextService* getInstance();

private:
    sf::Text mFps;
    sf::Text mParticlesCount;
    sf::Text mUpdateTime;

    sf::Font mFont;

    static DataTextService* sInstance;
};
