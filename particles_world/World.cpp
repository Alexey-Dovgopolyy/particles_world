#include "World.h"
#include "Math.h"
#include "common_data.h"
#include "common_types.h"
#include "ServiceProvider.h"

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
    debugInit();
}

void World::debugInit()
{
    ConfigService* config = ServiceProvider::getConfigService();
    const DebugConfig& debugConfig = config->getDebugConfig();

    Force f1(sf::Vector2f(debugConfig.debugP1DirX, debugConfig.debugP1DirY), debugConfig.debugP1Speed);
    Force f2(sf::Vector2f(debugConfig.debugP2DirX, debugConfig.debugP2DirY), debugConfig.debugP2Speed);

    Particle p1;
    p1.setPosition(sf::Vector2f(debugConfig.debugP1PosX, debugConfig.debugP1PosY));
    p1.setRadius(2.f);
    p1.applyForce(f1);
    p1.setColor(sf::Color::Green);

    Particle p2;
    p2.setPosition(sf::Vector2f(debugConfig.debugP2PosX, debugConfig.debugP2PosY));
    p2.setRadius(2.f);
    p2.applyForce(f2);

    mParticles.push_back(p1);
    mParticles.push_back(p2);
}

void World::debugCollision()
{
    Particle& particle1 = mParticles[0];
    Particle& particle2 = mParticles[1];

    const sf::Vector2f& particlePos1 = particle1.getPosition();
    const sf::Vector2f& particlePos2 = particle2.getPosition();

    sf::Vector2f attractDir = particlePos2 - particlePos1;
    float distance = Math::vectorLength(attractDir);

    ConfigService* config = ServiceProvider::getConfigService();

    float attractionRadius = config->getAttractionRadius();
    float attractionCoef = config->getAttractionCoef();
    float repelRadius = config->getRepelRadius();

    bool isAttracting = (distance < attractionRadius && distance > repelRadius);
    bool isRepelling = (distance < repelRadius);

    if (isAttracting)
    {
        float attractionForceAmount = attractionRadius - distance;
        attractionForceAmount *= attractionCoef;

        Force attractionForce(attractDir, attractionForceAmount);

        particle1.applyForce(attractionForce);

        attractionForce.setDirection(-1.f * attractDir);
        particle2.applyForce(attractionForce);
    }
    else if (isRepelling)
    {
        collide(particle1, particle2);

        float distanceDiff = repelRadius - distance;
        distanceDiff /= 2.f;

        sf::Vector2f moveP2 = Math::normalize(attractDir) * distanceDiff;
        sf::Vector2f moveP1 = (-1.f * moveP2) * distanceDiff;

        particle1.moveBy(moveP1);
        particle2.moveBy(moveP2);
    }
}

void World::update(float dt)
{
    debugCollision();

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
    sf::Vector2f moveVector1 = particle1.getMoveVector();
    sf::Vector2f moveVector2 = particle2.getMoveVector();

    sf::Vector2f collisionVector = particle2.getPosition() - particle1.getPosition();
    sf::Vector2f collisionPerpVector = Math::getPerp(collisionVector);

    Math::normalizeThis(collisionVector);
    Math::normalizeThis(collisionPerpVector);

    float dotCollision1 = Math::dotProduct(moveVector1, collisionVector);
    float dotCollision2 = Math::dotProduct(moveVector2, collisionVector);

    sf::Vector2f projMoveToCollision1 = dotCollision1 * collisionVector;
    sf::Vector2f projMoveToCollision2 = dotCollision2 * collisionVector;

    float dotPerpCollision1 = Math::dotProduct(moveVector1, collisionPerpVector);
    float dotPerpCollision2 = Math::dotProduct(moveVector2, collisionPerpVector);

    sf::Vector2f projPerpToCollision1 = dotPerpCollision1 * collisionPerpVector;
    sf::Vector2f projPerpToCollision2 = dotPerpCollision2 * collisionPerpVector;

    sf::Vector2f newDirection1 = projPerpToCollision1 + projMoveToCollision2;
    sf::Vector2f newDirection2 = projPerpToCollision2 + projMoveToCollision1;

    ConfigService* config = ServiceProvider::getConfigService();
    newDirection1 *= config->getRepelCoef();
    newDirection2 *= config->getRepelCoef();

    particle1.setMoveVector(newDirection1);
    particle2.setMoveVector(newDirection2);
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
