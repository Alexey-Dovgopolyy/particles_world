#pragma once
#include "Particle.h"

#include <vector>

class World
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

private:
    void createParticle(const sf::Vector2f& zoneCenter, float zoneRadius);

    static void collide(Particle& particle1, Particle& particle2);
    static sf::Vector2f calculateReflectVector(const sf::Vector2f& wall, Particle& particle);

private:
    std::vector<Particle*> mParticles;

    sf::CircleShape mSpawnZone;
    float mSpawnRadius = 70.f;
};

