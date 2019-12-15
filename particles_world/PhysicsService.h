#pragma once

#include "QuadTree.h"

#include <map>
#include <set>

class Particle;

class PhysicsService
{
    friend class ServiceProvider;

public:
    void clear();
    void insert(Particle* particle);
    void retrieve(std::vector<std::vector<Particle*>>& possibleCollisions, Particle* particle);

    void resolveCollisions();

    void draw();

    void applyForces();
    bool interaction(Particle& particle, Particle& other);
    void dealWithWalls(std::vector<Particle*>& particles);
    void applyGravity(std::vector<Particle*>& particles);

    static bool isParticlesInteract(Particle* particle1, Particle* particle2);
    void collide(Particle& particle1, Particle& particle2);
    static sf::Vector2f calculateReflectVector(const sf::Vector2f& wall, Particle& particle);

private:
    PhysicsService();
    PhysicsService(const PhysicsService& window) = delete;

    bool init();

    static void create();
    static void cleanup();
    static PhysicsService* getInstance();

private:
    QuadTree mQuadTree;
    std::set<Particle*> mCollisions;

    std::map<Particle*, sf::Vector2f> mForces;

    std::map<Particle*, std::set<Particle*>> mResolvedCollisions;

    static PhysicsService* sInstance;
};
