#include "Particle.h"
#include "ServiceProvider.h"
#include "Math.h"

Particle::Particle()
{
    mCircle.setFillColor(sf::Color::White);
    mAttractionCircle.setFillColor(sf::Color(46, 139, 87, 35));
    mRepelCircle.setFillColor(sf::Color(65, 105, 225, 70));

    float attractionRadius = ServiceProvider::getConfigService()->getAttractionRadius();

    sf::Vector2f centerPos = mPosition;

    mBounds.left = mPosition.x - attractionRadius;
    mBounds.top = mPosition.y - attractionRadius;
    mBounds.width = attractionRadius * 2.f;
    mBounds.height = attractionRadius * 2.f;
}

Particle::~Particle()
{
}

void Particle::setRadius(float radius)
{
    float attractionRadius = ServiceProvider::getConfigService()->getAttractionRadius();
    float repelRadius = ServiceProvider::getConfigService()->getRepelRadius();

    mCircle.setRadius(radius);
    mCircle.setOrigin(sf::Vector2f(radius, radius));

    mAttractionCircle.setRadius(attractionRadius);
    mAttractionCircle.setOrigin(sf::Vector2f(attractionRadius, attractionRadius));

    mRepelCircle.setRadius(repelRadius);
    mRepelCircle.setOrigin(sf::Vector2f(repelRadius, repelRadius));
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
    mAttractionCircle.setFillColor(sf::Color(46, 139, 87, 100));
    mRepelCircle.setFillColor(sf::Color(0, 0, 50, 40));
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

//     mAttractionCircle.setPosition(mPosition);
//     window->draw(mAttractionCircle);

//     mRepelCircle.setPosition(mPosition);
//     window->draw(mRepelCircle);

    mCircle.setPosition(mPosition);
    window->draw(mCircle);
}

const sf::FloatRect& Particle::getBoundingRect() const
{
    return mAttractionCircle.getGlobalBounds();
}
