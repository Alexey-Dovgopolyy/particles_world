#include "World.h"
#include "Math.h"
#include "common_data.h"

#include "SFML/Graphics/Color.hpp"

#include <iostream>
#include <cmath>

World::World()
{
}

World::~World()
{
}

void World::init()
{
    Force f1(sf::Vector2f(1.f, 0.f), 100.f);
    Force f2(sf::Vector2f(-1.f, 0.f), 0.f);

    Particle p1;
    p1.setPosition(sf::Vector2f(200.f, 300.f));
    p1.setRadius(10.f);
    p1.applyForce(f1);
    p1.setColor(sf::Color::Green);

    Particle p2;
    p2.setPosition(sf::Vector2f(500.f, 310.f));
    p2.setRadius(10.f);
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

    bool isAttracting = (distance < gAttractionRadius && distance > gRepelRadius);
    bool isRepelling = (distance < gRepelRadius);

    if (isAttracting)
    {
//         float attractionForceAmount = gAttractionRadius - distance;
//         attractionForceAmount *= gAttractionCoef;
// 
//         Force attractionForce(attractDir, attractionForceAmount);
// 
//         particle1.applyForce(attractionForce);
// 
//         attractionForce.setDirection(-1.f * attractDir);
//         particle2.applyForce(attractionForce);
    }
    else if (isRepelling)
    {
        collide2(particle1, particle2);

        float distanceDiff = gRepelRadius - distance;
        distanceDiff /= 2.f;

        sf::Vector2f moveP2 = Math::normalize(attractDir) * distanceDiff;
        sf::Vector2f moveP1 = (-1.f * moveP2) * distanceDiff;

        particle1.moveBy(moveP1);
        particle2.moveBy(moveP2);

        std::cout << distance << std::endl;
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

void World::createParticle(const sf::Vector2f& zoneCenter, float zoneRadius)
{

}

void World::collide(Particle& particle1, Particle& particle2)
{
    sf::Vector2f particlePos1 = particle1.getPosition();
    sf::Vector2f particlePos2 = particle2.getPosition();

    sf::Vector2f attractDir = particlePos2 - particlePos1;
    float distance = Math::vectorLength(attractDir);

    sf::Vector2f reflectionWall = Math::getPerp(attractDir);

    sf::Vector2f reflect1 = calculateReflectVector(reflectionWall, particle1);
    sf::Vector2f reflect2 = calculateReflectVector(reflectionWall, particle2);

    float angleCos1 = Math::dotProduct(particle1.getDirection(), reflect1);
    float angleCos2 = Math::dotProduct(particle2.getDirection(), reflect2);

    float angle1 = acosf(angleCos1) * (180.f / 3.14f);
    float angle2 = acosf(angleCos2) * (180.f / 3.14f);

    float impulsePercent = 1.f - (angle1 / 180.f);

    float newSpeed1 = particle1.getSpeed() * impulsePercent;
    float newSpeed2 = particle2.getSpeed() * impulsePercent;

    float distanceDiff = gRepelRadius - distance;
    distanceDiff /= 2.f;

    sf::Vector2f moveP2 = Math::normalize(attractDir) * distanceDiff;
    sf::Vector2f moveP1 = (-1.f * moveP2) * distanceDiff;

    particle1.moveBy(moveP1);
    particle2.moveBy(moveP2);
}

void World::collide2(Particle& particle1, Particle& particle2)
{
//     sf::Vector2f particlePos1 = particle1.getPosition();
//     sf::Vector2f particlePos2 = particle2.getPosition();
// 
//     sf::Vector2f forceVectorForParticle1 = particlePos1 - particlePos2;
//     sf::Vector2f forceVectorForParticle2 = particlePos2 - particlePos1;
// 
//     Force forceFor1(forceVectorForParticle1, particle2.getSpeed());
//     Force forceFor2(forceVectorForParticle2, particle1.getSpeed());
// 
//     particle1.applyForce(forceFor1);
//     particle2.applyForce(forceFor2);
}

sf::Vector2f World::calculateReflectVector(const sf::Vector2f& wall, Particle& particle)
{
    sf::Vector2f reflectVector;

    sf::Vector2f moveVector = particle.getMoveVector();
    sf::Vector2f wallPerp = Math::getPerp(wall);

    float vn = Math::dotProduct(moveVector, Math::normalize(wallPerp)) * 2.f;
    sf::Vector2f vnn = Math::normalize(wallPerp) * vn;
    reflectVector = moveVector - vnn;
    Math::normalizeThis(reflectVector);

    return reflectVector;
}
