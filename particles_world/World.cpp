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
#include "WorldService.h"

#include "SFML/Graphics/Color.hpp"

#include <iostream>
#include <cmath>
#include <random>

static size_t sMaxParticlesCount = 2000;

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
    ServiceProvider::getCommunicationService()->addListener(MessageType::incTime, this);
    ServiceProvider::getCommunicationService()->addListener(MessageType::decTime, this);

    initMessageHandlers();

    mSpawnZone.setRadius(mSpawnRadius);
    mSpawnZone.setFillColor(sf::Color::Transparent);
    mSpawnZone.setOutlineColor(sf::Color(100, 100, 100));
    mSpawnZone.setOutlineThickness(1.f);
    mSpawnZone.setOrigin(mSpawnRadius, mSpawnRadius);

    mParticles.reserve(sMaxParticlesCount);

    ServiceProvider::getDataTextService()->setInitialSpeed(0.f);

    return true;
}

void World::initMessageHandlers()
{
    mHandlers[MessageType::mouseWheelMoved] = &World::handleMouseWheelMoved;
    mHandlers[MessageType::mouseMoved] = &World::handleMouseMoved;
    mHandlers[MessageType::spawnParticle] = &World::handleSpawnParticle;
    mHandlers[MessageType::incInitialSpeed] = &World::handleIncInitialSpeed;
    mHandlers[MessageType::decInitialSpeed] = &World::handleDecInitialSpeed;
    mHandlers[MessageType::incAllSpeed] = &World::handleIncAllSpeed;
    mHandlers[MessageType::decAllSpeed] = &World::handleDecAllSpeed;
    mHandlers[MessageType::allFreeze] = &World::handleAllFreeze;
    mHandlers[MessageType::allFreezeInRad] = &World::handleAllFreezeInRad;
    mHandlers[MessageType::switchQuadTree] = &World::handleSwitchQuadTree;
    mHandlers[MessageType::incTime] = &World::handleIncTime;
    mHandlers[MessageType::decTime] = &World::handleDecTime;
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

int World::getUpdateTimes() const
{
    return mUpdateTimes;
}

void World::handleMessage(MessageType messageType, Message* message)
{
    auto it = mHandlers.find(messageType);
    if (it != mHandlers.end())
    {
        it->second(message);
    }
}


void World::handleMouseWheelMoved(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    if (MessageMouseWheelMove* wheelMessage = dynamic_cast<MessageMouseWheelMove*>(message))
    {
        bool isZoomIn = wheelMessage->isZoomIn();
        world->mSpawnRadius += (isZoomIn ? 5.f : -5.f);
        world->mSpawnRadius = std::max(0.f, world->mSpawnRadius);
        world->mSpawnZone.setRadius(world->mSpawnRadius);
        world->mSpawnZone.setOrigin(world->mSpawnRadius, world->mSpawnRadius);
    }
}

void World::handleMouseMoved(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    if (MessageMouseMove* moveMessage = dynamic_cast<MessageMouseMove*>(message))
    {
        sf::Vector2i mousePos = moveMessage->mMousePos;
        world->mSpawnZone.setPosition(sf::Vector2f(float(mousePos.x), float(mousePos.y)));
    }
}

void World::handleSpawnParticle(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    float spawnPeriod = ServiceProvider::getConfigService()->getParticleSpawnPeriod();

    if (world->mUpdateTime > spawnPeriod)
    {
        world->resetUpdateTime();
        world->createParticle(world->mSpawnZone.getPosition(), world->mSpawnRadius);
    }
}

void World::handleIncInitialSpeed(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    float step = ServiceProvider::getConfigService()->getSpeedIncStep();
    float maxSpeed = ServiceProvider::getConfigService()->getMaxInitialSpeed();
    world->mInitialParticleSpeed += step;
    world->mInitialParticleSpeed = std::min(maxSpeed, world->mInitialParticleSpeed);
    ServiceProvider::getDataTextService()->setInitialSpeed(world->mInitialParticleSpeed);
}

void World::handleDecInitialSpeed(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    float step = ServiceProvider::getConfigService()->getSpeedIncStep();
    world->mInitialParticleSpeed -= step;
    world->mInitialParticleSpeed = std::max(0.f, world->mInitialParticleSpeed);
    ServiceProvider::getDataTextService()->setInitialSpeed(world->mInitialParticleSpeed);
}

void World::handleIncAllSpeed(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    for (Particle* particle : world->mParticles)
    {
        float speed = particle->getSpeed();
        speed *= 1.1f;
        particle->setSpeed(speed);
    }
}

void World::handleDecAllSpeed(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    for (Particle* particle : world->mParticles)
    {
        float speed = particle->getSpeed();
        speed *= 0.9f;
        particle->setSpeed(speed);
    }
}

void World::handleAllFreeze(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    for (Particle* particle : world->mParticles)
    {
        particle->setSpeed(0.f);
    }
}

void World::handleAllFreezeInRad(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    if (MessageAllFreezeInRadius* freezeMessage = dynamic_cast<MessageAllFreezeInRadius*>(message))
    {
        float radius = freezeMessage->mRadius;
        sf::Vector2f centerPos = freezeMessage->mCenterPos;

        for (Particle* particle : world->mParticles)
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
}

void World::handleSwitchQuadTree(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();
    world->mNeedDrawQuadTree = !(world->mNeedDrawQuadTree);
}

void World::handleIncTime(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    world->mUpdateTimes++;
    world->mUpdateTimes = std::min(4, world->mUpdateTimes);
}

void World::handleDecTime(Message* message)
{
    World* world = ServiceProvider::getWorldService()->getWorld();

    world->mUpdateTimes--;
    world->mUpdateTimes = std::max(1, world->mUpdateTimes);
}

void World::createParticle(const sf::Vector2f& zoneCenter, float zoneRadius)
{
    if (mParticles.size() >= sMaxParticlesCount)
    {
        return;
    }

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
