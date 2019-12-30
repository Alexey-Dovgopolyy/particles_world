#include "World.h"
#include "Math.h"
#include "common_data.h"
#include "common_types.h"
#include "ServiceProvider.h"
#include "CommunicationService.h"
#include "ConfigService.h"
#include "PhysicsService.h"
#include "WindowService.h"
#include "DataTextService.h"

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

bool World::init()
{
    ServiceProvider::getCommunicationService()->addListener(MessageType::mouseWheelMoved, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::mouseDown, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::mouseMoved, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::mouseUp, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::spawnParticle, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::incInitialSpeed, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::decInitialSpeed, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::incAllSpeed, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::decAllSpeed, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::allFreeze, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::allFreezeInRad, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::switchQuadTree, this);

    mSpawnZone.setRadius(mSpawnRadius);
    mSpawnZone.setFillColor(sf::Color::Transparent);
    mSpawnZone.setOutlineColor(sf::Color(100, 100, 100));
    mSpawnZone.setOutlineThickness(1.f);
    mSpawnZone.setOrigin(mSpawnRadius, mSpawnRadius);

    mParticles.reserve(1000);

    ServiceProvider::getDataTextService()->setInitialSpeed(0.f);

    return true;
}

void World::cleanup()
{
    for (Particle* particle : mParticles)
    {
        delete particle;
    }
}

void World::update(float dt)
{
    accumulateUpdateTime(dt);

    PhysicsService* physics = ServiceProvider::getPhysicsService();
    physics->clear();
    
    for (Particle* particle : mParticles)
    {
        physics->insert(particle);
    }

    physics->resolveCollisions();
    physics->applyGravity(mParticles);
    physics->dealWithWalls(mParticles);
    physics->applyForces();
    
    for (Particle* particle : mParticles)
    {
        particle->update(dt);
    }
}

void World::draw()
{
    if (mNeedDrawQuadTree)
    {
        ServiceProvider::getPhysicsService()->draw();
    }

    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
    window->draw(mSpawnZone);

    for (Particle* particle : mParticles)
    {
        particle->draw();
    }
}

size_t World::getParticlesCount() const
{
    return mParticles.size();
}

float World::getAverageSpeed() const
{
    float result = 0.f;

    if (mParticles.empty())
    {
        return result;
    }

    float speedSum = 0.f;
    for (Particle* particle : mParticles)
    {
        speedSum += particle->getSpeed();
    }

    size_t particlesCount = getParticlesCount();
    result = speedSum / particlesCount;

    return result;
}

void World::handleMessage(MessageType messageType, Message* message)
{
    switch (messageType)
    {
    case MessageType::mouseWheelMoved:
    {
        if (MessageMouseWheelMove* wheelMessage = dynamic_cast<MessageMouseWheelMove*>(message))
        {
            bool isZoomIn = wheelMessage->isZoomIn();
            mSpawnRadius += (isZoomIn ? 5.f : -5.f);
            mSpawnRadius = std::max(0.f, mSpawnRadius);
            mSpawnZone.setRadius(mSpawnRadius);
            mSpawnZone.setOrigin(mSpawnRadius, mSpawnRadius);
        }

        break;
    }

    case MessageType::mouseMoved:
    {
        if (MessageMouseMove* moveMessage = dynamic_cast<MessageMouseMove*>(message))
        {
            sf::Vector2i mousePos = moveMessage->mMousePos;
            mSpawnZone.setPosition(sf::Vector2f(float(mousePos.x), float(mousePos.y)));
        }

        break;
    }

    case MessageType::spawnParticle:
    {
        float spawnPeriod = ServiceProvider::getConfigService()->getParticleSpawnPeriod();

        if (mUpdateTime > spawnPeriod)
        {
            resetUpdateTime();
            createParticle(mSpawnZone.getPosition(), mSpawnRadius);
        }

        break;
    }

    case MessageType::incInitialSpeed:
    {
        float step = ServiceProvider::getConfigService()->getSpeedIncStep();
        float maxSpeed = ServiceProvider::getConfigService()->getMaxInitialSpeed();
        mInitialParticleSpeed += step;
        mInitialParticleSpeed = std::min(maxSpeed, mInitialParticleSpeed);
        ServiceProvider::getDataTextService()->setInitialSpeed(mInitialParticleSpeed);

        break;
    }

    case MessageType::decInitialSpeed:
    {
        float step = ServiceProvider::getConfigService()->getSpeedIncStep();
        mInitialParticleSpeed -= step;
        mInitialParticleSpeed = std::max(0.f, mInitialParticleSpeed);
        ServiceProvider::getDataTextService()->setInitialSpeed(mInitialParticleSpeed);

        break;
    }

    case MessageType::incAllSpeed:
    {
        for (Particle* particle : mParticles)
        {
            float speed = particle->getSpeed();
            speed *= 1.1f;
            particle->setSpeed(speed);
        }

        break;
    }

    case MessageType::decAllSpeed:
    {
        for (Particle* particle : mParticles)
        {
            float speed = particle->getSpeed();
            speed *= 0.9f;
            particle->setSpeed(speed);
        }

        break;
    }

    case MessageType::allFreeze:
    {
        for (Particle* particle : mParticles)
        {
            particle->setSpeed(0.f);
        }

        break;
    }

    case MessageType::allFreezeInRad:
    {
        if (MessageAllFreezeInRadius* freezeMessage = dynamic_cast<MessageAllFreezeInRadius*>(message))
        {
            float radius = freezeMessage->mRadius;
            sf::Vector2f centerPos = freezeMessage->mCenterPos;

            for (Particle* particle : mParticles)
            {
                sf::Vector2f particlePos = particle->getPosition();
                sf::Vector2f vecToParticle = particlePos - centerPos;
                float quadDistance = Math::vectorLengthQuad(vecToParticle);

                float quadSpawnZoneRad = radius * radius;

                if (quadSpawnZoneRad >= quadDistance)
                {
                    particle->setSpeed(0.f);
                }
            }
        }

        break;
    }

    case MessageType::switchQuadTree:
    {
        mNeedDrawQuadTree = !mNeedDrawQuadTree;

        break;
    }

    default:
    {
        break;
    }
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

    float particleRadius = ServiceProvider::getConfigService()->getParticleRadius();
    float drawRadius = ServiceProvider::getConfigService()->getParticleDrawRadius();

    Particle* particle = new Particle();
    particle->setPosition(particlePos);
    particle->setRadius(drawRadius);
    particle->setDirection(randomVector);
    particle->setSpeed(mInitialParticleSpeed);

    if (mInitialParticleSpeed <= 0.1f)
    {
        ServiceProvider::getCommunicationService()->queueMessage(MessageType::allFreezeInRad, 
            new MessageAllFreezeInRadius(mSpawnRadius, mSpawnZone.getPosition()));
    }

    mParticles.push_back(particle);
}

void World::accumulateUpdateTime(float dt)
{
    mUpdateTime += dt;
}

void World::resetUpdateTime()
{
    mUpdateTime = 0.f;
}
