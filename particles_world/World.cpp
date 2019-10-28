#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::init()
{
    Force f1;
    f1.direction = sf::Vector2f(1.f, 0.f);
    f1.amount = 50.f;

    Force f2;
    f2.direction = sf::Vector2f(-1.f, 0.f);
    f2.amount = 50.f;

    Particle p1;
    p1.setPosition(sf::Vector2f(100.f, 270.f));
    p1.setRadius(3.f);
    p1.applyForce(f1);

    Particle p2;
    p2.setPosition(sf::Vector2f(700.f, 310.f));
    p2.setRadius(3.f);
    p2.applyForce(f2);

    mParticles.push_back(p1);
    mParticles.push_back(p2);
}

void World::update(float dt)
{
    for (Particle& particle : mParticles)
    {
        particle.update(dt);
    }
}

void World::draw()
{
    for (Particle& particle : mParticles)
    {
        particle.draw();
    }
}
