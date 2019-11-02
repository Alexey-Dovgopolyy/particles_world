#include "Particle.h"
#include "ServiceProvider.h"
#include "Math.h"

Particle::Particle()
{
    mCircle.setFillColor(sf::Color::White);
}

Particle::~Particle()
{
}

void Particle::setRadius(float radius)
{
    mCircle.setRadius(radius);
}

float Particle::getRadius() const
{
    return 0.0f;
}

void Particle::setPosition(const sf::Vector2f& position)
{
    mPosition = position;
}

const sf::Vector2f& Particle::getPosition() const
{
    return mPosition;
}

void Particle::setSpeed(float speed)
{
    mSpeed = speed;
}

float Particle::getSpeed() const
{
    return mSpeed;
}

void Particle::setDirection(const sf::Vector2f& direction)
{
    mDirection = direction;
    Math::normalizeThis(mDirection);
}

const sf::Vector2f& Particle::getDirection() const
{
    return mDirection;
}

void Particle::applyForce(const Force& force)
{
    sf::Vector2f moveVector = mDirection * mSpeed;
    const sf::Vector2f& forceVector = force.getForceVector();

    sf::Vector2f newVector = moveVector + forceVector;

    float newAmount = Math::vectorLength(newVector);

    setSpeed(newAmount);
    setDirection(newVector);
}

void Particle::update(float dt)
{
    sf::Vector2f offset;

    offset = mDirection * (mSpeed * dt);

    const sf::Vector2f currentPos = getPosition();
    sf::Vector2f newPos = currentPos + offset;
    setPosition(newPos);
}

void Particle::draw()
{
    sf::RenderWindow* window = ServiceProvider::getWindowProvider()->getWindow();

    mCircle.setPosition(mPosition);
    window->draw(mCircle);
}
