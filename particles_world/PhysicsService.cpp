#include "PhysicsService.h"
#include "ServiceProvider.h"
#include "ConfigService.h"
#include "Particle.h"

#include <SFML/Graphics/Rect.hpp>

#include <iostream>
#include <cmath>

PhysicsService* PhysicsService::sInstance = nullptr;

PhysicsService::PhysicsService()
{

}

void PhysicsService::clear()
{
    mQuadTree.clear();
    mResolvedCollisions.clear();
    mForces.clear();
}

void PhysicsService::insert(Particle* particle)
{
    mQuadTree.insert(particle);
}

void PhysicsService::retrieve(std::vector<std::vector<Particle*>>& possibleCollisions, Particle* particle)
{
    mQuadTree.retrieve(possibleCollisions);
}

void PhysicsService::resolveCollisions()
{
    std::vector<std::vector<Particle*>> possibleCollisions;

    mQuadTree.retrieve(possibleCollisions);

    for (std::vector<Particle*>& particles : possibleCollisions)
    {
        for (size_t i = 0; i < particles.size(); i++)
        {
            for (size_t j = i + 1; j < particles.size(); j++)
            {
                Particle* p1 = particles[i];
                Particle* p2 = particles[j];

                bool isCollisionWasResolved = false;

                auto it = mResolvedCollisions.find(p1);
                if (it != mResolvedCollisions.end())
                {
                    const std::set<Particle*>& resolved = it->second;
                    isCollisionWasResolved = (resolved.find(p2) != resolved.end());
                }

                if (!isCollisionWasResolved)
                {
                    if (interaction(*p1, *p2))
                    {
                        mResolvedCollisions[p1].insert(p2);
                    }
                }
            }
        }
    }
}

void PhysicsService::draw()
{
    mQuadTree.draw();
}

bool PhysicsService::interaction(Particle& particle1, Particle& particle2)
{
    const sf::Vector2f& particlePos1 = particle1.getPosition();
    const sf::Vector2f& particlePos2 = particle2.getPosition();

    sf::Vector2f vecToP1 = particlePos1 - particlePos2;
    sf::Vector2f vecToP2 = particlePos2 - particlePos1;

    float distance = Math::vectorLength(vecToP1);

    ConfigService* config = ServiceProvider::getConfigService();

    float particleRadius = config->getParticleRadius();

    float eCoef = config->getECoef();
    int repelPow = config->getRepelPow();
    int attractPow = config->getAttractPow();
    float attractRadius = config->getAttractionRadius();
    float collideRadius = config->getCollideRadius();
    float noForceDistCoef = config->getNoForceDistCoef();

    float noForceRadius = particleRadius * noForceDistCoef;
    float valCoef = noForceRadius / distance;

    if (distance > particleRadius * attractRadius)
    {
        return false;
    }

    if (distance <= particleRadius * collideRadius)
    {
        collide(particle1, particle2);

        float distanceDiff = (particleRadius * collideRadius) - distance;
        distanceDiff /= 2.f;

        sf::Vector2f moveP2 = Math::normalize(vecToP2) * distanceDiff;
        sf::Vector2f moveP1 = (-1.f * moveP2) * distanceDiff;

        particle1.moveBy(moveP1);
        particle2.moveBy(moveP2);

        return true;
    }

    float forceAmount = eCoef * (pow(valCoef, repelPow) - 2.f * pow(valCoef, attractPow));

    Force repelForce1(vecToP1, forceAmount);
    Force repelForce2(vecToP2, forceAmount);

    mForces[&particle1] += repelForce1.getForceVector();
    mForces[&particle2] += repelForce2.getForceVector();

    return true;
}

void PhysicsService::dealWithWalls(std::vector<Particle*>& particles)
{
    ConfigService* config = ServiceProvider::getConfigService();
    float width = static_cast<float>(config->getWinSizeX());
    float height = static_cast<float>(config->getWinSizeY());
    float particleRad = config->getParticleRadius();

    for (Particle* particle : particles)
    {
        bool isWallHitted = false;

        sf::Vector2f pos = particle->getPosition();
        if (pos.y + particleRad >= height)
        {
            float forceAmount = (pos.y + particleRad - height);
            Force force;
            force.setDirection(sf::Vector2f(0.f, -1.f));
            force.setAmount(forceAmount);

            mForces[particle] += force.getForceVector();
        }
        else if (pos.y - particleRad <= 0.f)
        {
            float forceAmount = (pos.y - particleRad);
            Force force;
            force.setDirection(sf::Vector2f(0.f, 1.f));
            force.setAmount(-forceAmount);

            mForces[particle] += force.getForceVector();
        }
        else if (pos.x + particleRad >= width)
        {
            float forceAmount = (pos.x + particleRad - width);
            Force force;
            force.setDirection(sf::Vector2f(-1.f, 0.f));
            force.setAmount(forceAmount);

            mForces[particle] += force.getForceVector();
        }
        else if (pos.x - particleRad <= 0.f)
        {
            float forceAmount = (pos.x - particleRad);
            Force force;
            force.setDirection(sf::Vector2f(1.f, 0.f));
            force.setAmount(-forceAmount);

            mForces[particle] += force.getForceVector();
        }
    }
}

void PhysicsService::applyGravity(std::vector<Particle*>& particles)
{
    float gravitationForce = ServiceProvider::getConfigService()->getGravitation();

    for (Particle* particle : particles)
    {
        Force gravitation;
        gravitation.setDirection(sf::Vector2f(0.f, 1.f));
        gravitation.setAmount(gravitationForce);

        mForces[particle] += gravitation.getForceVector();
    }
}

bool PhysicsService::isParticlesInteract(Particle* particle1, Particle* particle2)
{
    const sf::Vector2f& particlePos1 = particle1->getPosition();
    const sf::Vector2f& particlePos2 = particle2->getPosition();

    sf::Vector2f attractDir = particlePos2 - particlePos1;
    float distance = Math::vectorLength(attractDir);

    ConfigService* config = ServiceProvider::getConfigService();

    float particleRadius = config->getParticleRadius();
    float attractionRadius = config->getAttractionRadius();

    bool result = (distance <= attractionRadius);
    return result;
}

void PhysicsService::collide(Particle& particle1, Particle& particle2)
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

    particle1.setMoveVector(newDirection1);
    particle2.setMoveVector(newDirection2);
}

sf::Vector2f PhysicsService::calculateReflectVector(const sf::Vector2f& wall, Particle& particle)
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

void PhysicsService::applyForces()
{
    for (auto& data : mForces)
    {
        Particle* particle = data.first;
        const sf::Vector2f& forceVector = data.second;
        Force force(forceVector);
        particle->applyForce(force);
    }
}

bool PhysicsService::init()
{
    int width = ServiceProvider::getConfigService()->getWinSizeX();
    int height = ServiceProvider::getConfigService()->getWinSizeY();

    sf::FloatRect quadTreeBounds;
    quadTreeBounds.width = static_cast<float>(width);
    quadTreeBounds.height = static_cast<float>(height);

    mQuadTree.setBounds(quadTreeBounds);
    mQuadTree.init();

    return true;
}

void PhysicsService::create()
{
    sInstance = new PhysicsService();
}

void PhysicsService::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

PhysicsService* PhysicsService::getInstance()
{
    return sInstance;
}
