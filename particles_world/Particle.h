#pragma once

#include "SFML/Graphics/CircleShape.hpp"

#include "Force.h"

class Particle
{
public:
    Particle();
    ~Particle();

    void setRadius(float radius);
    float getRadius() const;

    void setPosition(const sf::Vector2f& position);
    const sf::Vector2f& getPosition() const;

    void setSpeed(float speed);
    float getSpeed() const;

    void setDirection(sf::Vector2f direction);
    const sf::Vector2f& getDirection() const;

    void applyForce(const Force& force);

    void update(float dt);
    void draw();

private:
    sf::CircleShape mCircle;
    sf::Vector2f mPosition;
    sf::Vector2f mDirection;
    float mSpeed = 0.f;
};

