#pragma once

#include "SFML/System/Vector2.hpp"
#include "Math.h"

class Force
{
public:
    Force();
    Force(const sf::Vector2f& direction, float amount);
    Force(const sf::Vector2f& forceVector);
    ~Force();

    void setDirection(const sf::Vector2f& direction);
    const sf::Vector2f& getDirection() const;

    void setAmount(float amount);
    float getAmount() const;

    void setForce(const sf::Vector2f& force);
    const sf::Vector2f& getForceVector() const;

private:
    void calculateForceVector();

private:
    sf::Vector2f mDirection;
    float mAmount;
    sf::Vector2f mForceVector;
};

