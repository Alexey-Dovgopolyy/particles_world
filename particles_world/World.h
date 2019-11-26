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
    void debugInit();
    void cleanup();

    void debugCollision(Particle& particle1, Particle& particle2);

    void update(float dt);
    void draw();

    void handleMessage(MessageType messageType, Message* message);

private:
    void createParticle(const sf::Vector2f& zoneCenter, float zoneRadius);
    void accumulateUpdateTime(float dt);
    void resetUpdateTime();

    static void collide(Particle& particle1, Particle& particle2);
    static sf::Vector2f calculateReflectVector(const sf::Vector2f& wall, Particle& particle);

private:
    std::vector<Particle*> mParticles;

    float mUpdateTime = 0.f;

    sf::CircleShape mSpawnZone;
    float mSpawnRadius = 30.f;
};

