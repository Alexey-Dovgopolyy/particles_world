#include "Particle.h"
#include "ServiceProvider.h"
#include "ConfigService.h"
#include "WindowService.h"
#include "Math.h"

#include <SFML/Graphics/RectangleShape.hpp>

Particle::Particle()
{
    mCircle.setFillColor(sf::Color::White);

    float attractionRadius = ServiceProvider::getConfigService()->getAttractionRadius();
    float particleRad = ServiceProvider::getConfigService()->getParticleRadius();

    sf::Vector2f centerPos = mPosition;

    mBounds.left = mPosition.x - particleRad * attractionRadius;
    mBounds.top = mPosition.y - particleRad * attractionRadius;
    mBounds.width = attractionRadius * particleRad * 2.f;
    mBounds.height = attractionRadius * particleRad * 2.f;
}

Particle::~Particle()
{
}

void Particle::setRadius(float radius)
{
    float attractionRad = ServiceProvider::getConfigService()->getAttractionRadius();
    float particleRad = ServiceProvider::getConfigService()->getParticleRadius();

    mCircle.setRadius(radius);
    mCircle.setOrigin(sf::Vector2f(radius, radius));

    mBounds.left = mPosition.x - particleRad * attractionRad;
    mBounds.top = mPosition.y - particleRad * attractionRad;
    mBounds.width = attractionRad * particleRad * 2.f;
    mBounds.height = attractionRad * particleRad * 2.f;
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

//     const sf::FloatRect& rect = getBoundingRect();
    
//     sf::RectangleShape debugRectangle;
//     debugRectangle.setFillColor(sf::Color::Transparent);
//     debugRectangle.setOutlineColor(sf::Color::Green);
//     debugRectangle.setOutlineThickness(1.f);
//     debugRectangle.setPosition(sf::Vector2f(rect.left, rect.top));
//     debugRectangle.setSize(sf::Vector2f(rect.width, rect.height));
// 
//     window->draw(debugRectangle);

    const int maxSpeed = ServiceProvider::getConfigService()->getMaxInitialSpeed() * 0.7f;
    const float maxSpeedPercent = std::min(1.f, mSpeed / maxSpeed);
    const int colorLerp = static_cast<int>(Math::lerp(0, 255, maxSpeedPercent));
    const int bluePart = 255 - colorLerp;

    //sf::Color circle(colorLerp, 0, 255);
    sf::Color circle(colorLerp, colorLerp, bluePart);
    mCircle.setFillColor(circle);
    mCircle.setPosition(mPosition);
    window->draw(mCircle);
}

const sf::FloatRect& Particle::getBoundingRect()
{
    float attractionRad = ServiceProvider::getConfigService()->getAttractionRadius();
    float particleRad = ServiceProvider::getConfigService()->getParticleRadius();

    mBounds.left = mPosition.x - particleRad * attractionRad;
    mBounds.top = mPosition.y - particleRad * attractionRad;

    return mBounds;
}
