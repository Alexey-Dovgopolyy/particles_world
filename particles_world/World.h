#pragma once
#include "Particle.h"
#include "MessageListener.h"

#include <vector>

class World :
    public MessageListener
{
public:
    World();
    ~World();

    bool init();
    void cleanup();

    void update(float dt);
    void draw();

    size_t getParticlesCount() const;
    float getAverageSpeed() const;

    void handleMessage(MessageType messageType, Message* message);

private:
    void createParticle(const sf::Vector2f& zoneCenter, float zoneRadius);
    void accumulateUpdateTime(float dt);
    void resetUpdateTime();

private:
    std::vector<Particle*> mParticles;

    float mUpdateTime = 0.f;

    sf::CircleShape mSpawnZone;
    float mSpawnRadius = 30.f;

    float mInitialParticleSpeed = 0.f;

    bool mNeedDrawQuadTree = false;
};

