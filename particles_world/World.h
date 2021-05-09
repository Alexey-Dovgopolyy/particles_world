#pragma once
#include "Particle.h"
#include "MessageListener.h"

#include <vector>
#include <map>

class World :
    public MessageListener
{
public:
    World();
    ~World();

    bool init();
    void initMessageHandlers();
    void cleanup();

    void update(float dt);
    void draw();

    size_t getParticlesCount() const;
    float getAverageSpeed() const;

    int getUpdateTimes() const;

    //void handleMessage(MessageType messageType, Message* message);

    // TODO set handlers into proper classes
    static void handleMouseWheelMoved(Message* message);
    static void handleMouseMoved(Message* message);
    static void handleSpawnParticle(Message* message);
    static void handleIncInitialSpeed(Message* message);
    static void handleDecInitialSpeed(Message* message);
    static void handleIncAllSpeed(Message* message);
    static void handleDecAllSpeed(Message* message);
    static void handleAllFreeze(Message* message);
    static void handleAllFreezeInRad(Message* message);
    static void handleSwitchQuadTree(Message* message);
    static void handleIncTime(Message* message);
    static void handleDecTime(Message* message);
    static void handleSetSpeedInRad(Message* message);
    static void handleSwitchCreateHeat(Message* message);

private:
    void createParticle(const sf::Vector2f& zoneCenter, float zoneRadius);
    void accumulateUpdateTime(float dt);
    void resetUpdateTime();

private:
    std::vector<Particle> mParticles;

    float mUpdateTime = 0.f;

    sf::CircleShape mSpawnZone;
    float mSpawnRadius = 30.f;

    float mInitialParticleSpeed = 0.f;

    bool mNeedDrawQuadTree = false;
    bool mCreateParticlesMode = true;

    int mUpdateTimes = 1;

    //std::map<MessageType, std::function<void(Message*)>> mHandlers;
};

