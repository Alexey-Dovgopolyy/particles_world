#pragma once
#include "Particle.h"

#include <vector>

class World
{
public:
    World();
    ~World();

    void init();

    void update(float dt);
    void draw();

private:
    std::vector<Particle> mParticles;
};

