#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

#include "MessageListener.h"

class DataTextService
    : public MessageListener
{
    friend class ServiceProvider;

public:
    void update(/*float dt*/);
    void draw();

    void setFps(int fps);
    void setUpdateTime(float time);
    void setInitialSpeed(float initSpeed);
    void setAttractionPow();
    void setRepelPow();

private:
    DataTextService();
    DataTextService(const DataTextService& window) = delete;

    bool init();

    static void handleIncAttraction(Message* message);
    static void handleDecAttraction(Message* message);
    static void handleIncRepelling(Message* message);
    static void handleDecRepelling(Message* message);

    static void create();
    static void cleanup();
    static DataTextService* getInstance();

private:
    sf::Text mFps;
    sf::Text mParticlesCount;
    sf::Text mUpdateTime;
    sf::Text mInitSpeed;
    sf::Text mEnergy;
    sf::Text mGravity;
    sf::Text mUpdateTimes;

    sf::Text mIncSpeed;
    sf::Text mDecSpeed;
    sf::Text mFreeze;
    sf::Text mQuadTree;
    sf::Text mIncDecTimes;
    sf::Text mCreateHeatMode;
    sf::Text mAttractPow;
    sf::Text mRepelPow;

    sf::Font mFont;

    static DataTextService* sInstance;
};
