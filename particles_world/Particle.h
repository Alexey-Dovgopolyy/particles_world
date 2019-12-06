#pragma once

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"

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

    void setDirection(const sf::Vector2f& direction);
    const sf::Vector2f& getDirection() const;

    void setColor(sf::Color color);

    void setMoveVector(const sf::Vector2f& moveVector);
    sf::Vector2f getMoveVector() const;

    void applyForce(const Force& force);
    void moveBy(const sf::Vector2f& move);

    void update(float dt);
    void draw();

    const sf::FloatRect& getBoundingRect() const;

private:
    sf::CircleShape mCircle;
    sf::CircleShape mAttractionCircle;
    sf::CircleShape mRepelCircle;
    sf::Vector2f mPosition;
    sf::Vector2f mDirection;
    float mSpeed = 0.f;
    sf::FloatRect mBounds;
};

