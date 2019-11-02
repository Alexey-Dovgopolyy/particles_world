#include "World.h"
#include "Math.h"
#include "common_data.h"

World::World()
{
}

World::~World()
{
}

void World::init()
{
    Force f1(sf::Vector2f(1.f, 0.f), 180.f);
    Force f2(sf::Vector2f(-1.f, 0.f), 0.f);

    Particle p1;
    p1.setPosition(sf::Vector2f(100.f, 270.f));
    p1.setRadius(3.f);
    p1.applyForce(f1);

    Particle p2;
    p2.setPosition(sf::Vector2f(400.f, 310.f));
    p2.setRadius(3.f);
    p2.applyForce(f2);

    mParticles.push_back(p1);
    mParticles.push_back(p2);
}

void World::update(float dt)
{
    Particle& particle1 = mParticles[0];
    Particle& particle2 = mParticles[1];

    const sf::Vector2f& particlePos1 = particle1.getPosition();
    const sf::Vector2f& particlePos2 = particle2.getPosition();

    sf::Vector2f attractDir = particlePos2 - particlePos1;
    float distance = Math::vectorLength(attractDir);

    if (distance < gAttractionRadius)
    {
        float attractionForceAmount = gAttractionRadius - distance;
        attractionForceAmount *= gAttractionCoef;

        Force attractionForce(attractDir, attractionForceAmount);

        particle1.applyForce(attractionForce);

        attractionForce.setDirection(-1.f * attractDir);
        particle2.applyForce(attractionForce);
    }

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
