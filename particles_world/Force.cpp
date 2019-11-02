#include "Force.h"
#include "Math.h"

Force::Force()
{
}

Force::Force(const sf::Vector2f& direction, float amount)
{
    mDirection = Math::normalize(direction);
    mAmount = amount;
    calculateForceVector();
}

Force::~Force()
{
}

void Force::setDirection(const sf::Vector2f& direction)
{
    mDirection = Math::normalize(direction);
    calculateForceVector();
}

const sf::Vector2f& Force::getDirection() const
{
    return mDirection;
}

void Force::setAmount(float amount)
{
    mAmount = amount;
    calculateForceVector();
}

float Force::getAmount() const
{
    return mAmount;
}

const sf::Vector2f& Force::getForceVector() const
{
    return mForceVector;
}

void Force::calculateForceVector()
{
    mForceVector = mDirection * mAmount;
}
