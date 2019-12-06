#include "PhysicsService.h"
#include "ServiceProvider.h"
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
}

void PhysicsService::insert(Particle* particle)
{
    mQuadTree.insert(particle);
}

void PhysicsService::retrieve(std::set<Particle*>& possibleCollisions, Particle* particle)
{
    mQuadTree.retrieve(possibleCollisions, particle);
}

void PhysicsService::retrievePossibleCollisions(Particle* particle)
{
    std::set<Particle*> possibleCollisions;

    mQuadTree.retrieve(possibleCollisions, particle);

    for (Particle* particleToCheck : possibleCollisions)
    {
        //bool iteract = isIteract(particle, particleToCheck);
        bool isCollisionWasResolved = false;

        auto it = mResolvedCollisions.find(particleToCheck);
        if (it != mResolvedCollisions.end())
        {
            const std::set<Particle*>& resolved = it->second;
            isCollisionWasResolved = (resolved.find(particle) != resolved.end());
        }
        
        bool needResolve = (/*iteract && */!isCollisionWasResolved && particle != particleToCheck);
        if (needResolve)
        {
            interaction(*particle, *particleToCheck);
        }

        mResolvedCollisions[particle].insert(particleToCheck);
    }
}

void PhysicsService::resolveCollisions(std::vector<Particle*>& particles)
{
    mForces.clear();

    for (size_t i = 0; i < particles.size(); i++)
    {
        for (size_t j = i + 1; j < particles.size(); j++)
        {
            Particle* p1 = particles[i];
            Particle* p2 = particles[j];

            if (isInteract(p1, p2))
            {
                interaction_2(*p1, *p2);
            }
        }
    }
}

void PhysicsService::draw()
{
    mQuadTree.drawCurrent();
}

void PhysicsService::interaction_2(Particle& particle1, Particle& particle2)
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

    float noForceRadius = particleRadius * 2.f;
    float valCoef = noForceRadius / distance;

    float forceAmount = 4.f * eCoef * (pow(valCoef, repelPow) - pow(valCoef, attractPow));

    Force repelForce1(vecToP1, forceAmount);
    Force repelForce2(vecToP2, forceAmount);

    mForces[&particle1] = mForces[&particle1] + repelForce1.getForceVector();
    mForces[&particle2] = mForces[&particle2] + repelForce2.getForceVector();

//     particle1.applyForce(repelForce1);
//     particle2.applyForce(repelForce2);
}

void PhysicsService::interaction(Particle& particle1, Particle& particle2)
{
    const sf::Vector2f& particlePos1 = particle1.getPosition();
    const sf::Vector2f& particlePos2 = particle2.getPosition();

    sf::Vector2f attractDir = particlePos2 - particlePos1;
    float distance = Math::vectorLength(attractDir);

    ConfigService* config = ServiceProvider::getConfigService();

    float attractionRadius = config->getAttractionRadius();
    float attractionCoef = config->getAttractionCoef();
    float repelRadius = config->getRepelRadius();
    float repelCoef = config->getRepelCoef();
    float particleRadius = config->getParticleRadius();

    bool isAttracting = (distance <= attractionRadius + particleRadius);
    bool isRepelling = (distance <= repelRadius + particleRadius);
    bool isCollide = (distance <= (particleRadius * 2.f));

//     bool isAttracting = (distance - particleRadius <= attractionRadius && distance > repelRadius);
//     bool isRepelling = (distance <= repelRadius + particleRadius && distance > (particleRadius * 2.f));
//     bool isCollide = (distance <= (particleRadius * 2.f));

    if (isAttracting)
    {
        float attractionForceAmount = attractionRadius + particleRadius - distance;
        attractionForceAmount *= attractionForceAmount;
        attractionForceAmount *= attractionCoef;

        //attractionForceAmount = 3.f;
        Force attractionForce(attractDir, attractionForceAmount);

        particle1.applyForce(attractionForce);

        attractionForce.setDirection(-1.f * attractDir);
        particle2.applyForce(attractionForce);
    }
     if (isRepelling)
    {
        sf::Vector2f repellingVector1 = particlePos1 - particlePos2;
        sf::Vector2f repellingVector2 = particlePos2 - particlePos1;

        float repellingForceAmount = repelRadius + particleRadius - distance;
        repellingForceAmount *= repellingForceAmount;
        repellingForceAmount *= repelCoef;

        //repellingForceAmount = 1.f;

        Force repelForce1(repellingVector1, repellingForceAmount);
        Force repelForce2(repellingVector2, repellingForceAmount);

        particle1.applyForce(repelForce1);
        particle2.applyForce(repelForce2);

        std::cout << "repel " << repellingForceAmount << std::endl;
    }
     if (isCollide)
    {
        collide(particle1, particle2);

        float distanceDiff = (particleRadius * 2.f) - distance;
        distanceDiff /= 2.f;

        sf::Vector2f moveP2 = Math::normalize(attractDir) * distanceDiff;
        sf::Vector2f moveP1 = (-1.f * moveP2) * distanceDiff;

        particle1.moveBy(moveP1);
        particle2.moveBy(moveP2);

        std::cout << "collide" << std::endl;
    }
}

bool PhysicsService::isInteract(Particle* particle1, Particle* particle2)
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

    ConfigService* config = ServiceProvider::getConfigService();

    float p1d = Math::vectorLength(moveVector1);
    float p2d = Math::vectorLength(moveVector2);
    float np1d = Math::vectorLength(newDirection1);
    float np2d = Math::vectorLength(newDirection2);

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
    float width = ServiceProvider::getConfigService()->getWinSizeX();
    float height = ServiceProvider::getConfigService()->getWinSizeY();

    sf::FloatRect quadTreeBounds;
    quadTreeBounds.width = width;
    quadTreeBounds.height = height;

    mQuadTree.setBounds(quadTreeBounds);

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
