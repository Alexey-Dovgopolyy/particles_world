#include "Particle.h"
#include "ServiceProvider.h"

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
    mCircle.setPosition(position);
    mPosition = position;
}

const sf::Vector2f& Particle::getPosition() const
{
    return mPosition;
}

void Particle::setSpeed(float speed)
{
}

float Particle::getSpeed() const
{
    return 0.0f;
}

void Particle::setDirection(sf::Vector2f direction)
{
}

const sf::Vector2f& Particle::getDirection() const
{
    return sf::Vector2f();
}

void Particle::applyForce(const Force& force)
{

}

void Particle::update(float dt)
{
}

void Particle::draw()
{
    sf::RenderWindow* window = ServiceProvider::getWindowProvider()->getWindow();

    window->draw(mCircle);
}
