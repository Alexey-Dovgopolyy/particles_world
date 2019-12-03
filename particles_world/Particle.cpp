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

void Particle::setColor(sf::Color color)
{
    mCircle.setFillColor(color);
}

void Particle::setMoveVector(const sf::Vector2f& moveVector)
{
    mSpeed = Math::vectorLength(moveVector);
    setDirection(moveVector);
}

sf::Vector2f Particle::getMoveVector() const
{
    sf::Vector2f moveVector;
    moveVector = mDirection * mSpeed;
    return moveVector;
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

void Particle::moveBy(const sf::Vector2f& move)
{
    mPosition += move;
    mCircle.setPosition(mPosition);
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
    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();

    mCircle.setPosition(mPosition);
    window->draw(mCircle);
}

sf::FloatRect Particle::getBoundingRect() const
{
    float attractionRadius = ServiceProvider::getConfigService()->getAttractionRadius();

    sf::Vector2f centerPos = mPosition;

    sf::FloatRect rect;
    rect.left = mPosition.x - attractionRadius;
    rect.top = mPosition.y - attractionRadius;
    rect.width = attractionRadius * 2.f;
    rect.height = attractionRadius * 2.f;

    return rect;
}
