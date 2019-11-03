#pragma once
#include "Particle.h"

#include <vector>

class World
{
public:
    World();
    ~World();

    void init();

    void update(float dt);
    void draw();

private:
    void createParticle(const sf::Vector2f& zoneCenter, float zoneRadius);

    static void collide(Particle& particle1, Particle& particle2);
    static void collide2(Particle& particle1, Particle& particle2);
    static sf::Vector2f calculateReflectVector(const sf::Vector2f& wall, Particle& particle);

private:
    std::vector<Particle> mParticles;
};

