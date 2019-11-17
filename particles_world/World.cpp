#include "World.h"
#include "Math.h"
#include "common_data.h"
#include "common_types.h"
#include "ServiceProvider.h"

#include "SFML/Graphics/Color.hpp"

#include <iostream>
#include <cmath>
#include <random>

World::World()
{
}

World::~World()
{
}

void World::init()
{
    mSpawnZone.setRadius(mSpawnRadius);
    mSpawnZone.setFillColor(sf::Color::Transparent);
    mSpawnZone.setOutlineColor(sf::Color(100, 100, 100));
    //mSpawnZone.setOutlineColor(sf::Color::Cyan);
    mSpawnZone.setOutlineThickness(1.f);
    float origin = mSpawnRadius / 2.f;
    mSpawnZone.setOrigin(origin, origin);

    debugInit();

    for (int i = 0; i < 10; i++)
    {
        createParticle(mSpawnZone.getPosition(), mSpawnRadius);
    }
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

    //mParticles.push_back(p1);
    //mParticles.push_back(p2);

    mSpawnZone.setPosition(debugConfig.debugSpawnPosX, debugConfig.debugSpawnPosY);
}

void World::cleanup()
{
    for (Particle* particle : mParticles)
    {
        delete particle;
    }
}

void World::debugCollision(Particle& particle1, Particle& particle2)
{
//     Particle& particle1 = mParticles[0];
//     Particle& particle2 = mParticles[1];

    const sf::Vector2f& particlePos1 = particle1.getPosition();
    const sf::Vector2f& particlePos2 = particle2.getPosition();

    sf::Vector2f attractDir = particlePos2 - particlePos1;
    float distance = Math::vectorLength(attractDir);

    ConfigService* config = ServiceProvider::getConfigService();

    float attractionRadius = config->getAttractionRadius();
    float attractionCoef = config->getAttractionCoef();
    float repelRadius = config->getRepelRadius();
    float repelCoef = config->getRepelCoef();
    float particleRadius = 2.f;

    bool isAttracting = (distance < attractionRadius && distance > repelRadius);
    bool isRepelling = (distance < repelRadius && distance > (particleRadius * 2.f));
    bool isCollide = (distance < (particleRadius * 2.f));

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
        sf::Vector2f repellingVector1 = particlePos1 - particlePos2;
        sf::Vector2f repellingVector2 = particlePos2 - particlePos1;

        float repellingForceAmount = repelRadius - distance;
        repellingForceAmount *= repelCoef;

        Force repelForce1(repellingVector1, repellingForceAmount);
        Force repelForce2(repellingVector2, repellingForceAmount);

        particle1.applyForce(repelForce1);
        particle2.applyForce(repelForce2);
    }
    else if (isCollide)
    {
        collide(particle1, particle2);

        float distanceDiff = (particleRadius * 2.f) - distance;
        distanceDiff /= 2.f;

        sf::Vector2f moveP2 = Math::normalize(attractDir) * distanceDiff;
        sf::Vector2f moveP1 = (-1.f * moveP2) * distanceDiff;

        particle1.moveBy(moveP1);
        particle2.moveBy(moveP2);
    }
}

void World::update(float dt)
{
    for (size_t i = 0; i < mParticles.size(); i++)
    {
        for (size_t j = i + 1; j < mParticles.size(); j++)
        {
            Particle& particle1 = *(mParticles[i]);
            Particle& particle2 = *(mParticles[j]);

            debugCollision(particle1, particle2);
        }
    }

    for (Particle* particle : mParticles)
    {
        particle->update(dt);
    }
}

void World::draw()
{
    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
    window->draw(mSpawnZone);

    for (Particle* particle : mParticles)
    {
        particle->draw();
    }
}

void World::createParticle(const sf::Vector2f& zoneCenter, float zoneRadius)
{
    int randomDistance = static_cast<int>(zoneRadius);

    float randomX = static_cast<float>(Math::randomInt(0, randomDistance));
    float randomY = static_cast<float>(Math::randomInt(0, randomDistance));

    bool negateX = static_cast<bool>(Math::randomInt(0, 1));
    bool negateY = static_cast<bool>(Math::randomInt(0, 1));

    randomX = (negateX ? -randomX : randomX);
    randomY = (negateY ? -randomY : randomY);

    sf::Vector2f randomVector(randomX, randomY);
    Math::normalizeThis(randomVector);

    float randomLength = static_cast<float>(Math::randomInt(0, randomDistance));

    randomVector *= randomLength;

    sf::Vector2f particlePos = zoneCenter + randomVector;

    static int colorBluePart = 255;

    Particle* particle = new Particle();
    particle->setPosition(particlePos);
    particle->setRadius(2.f);
    particle->setColor(sf::Color(colorBluePart, 255, colorBluePart));

    mParticles.push_back(particle);

    colorBluePart -= 20;
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
