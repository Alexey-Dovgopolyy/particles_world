#pragma once

#include <SFML/System/Vector2.hpp>

class Message
{
public:
    Message() {}
    virtual ~Message() {};
};

class MessageMouseWheelMove : public Message
{
public:
    MessageMouseWheelMove(float delta = 0.f) :
        mDelta(delta)
    {}

    bool isZoomIn() const
    {
        bool result = (mDelta > 0.f);
        return result;
    }

public:
    float mDelta = 0.f;
};

class MessageMouseMove : public Message
{
public:
    MessageMouseMove(sf::Vector2i& mousePos) :
        mMousePos(mousePos)
    {}

public:
    sf::Vector2i mMousePos;
};

class MessageSpawnParticle : public Message
{
public:
    MessageSpawnParticle() {}
};

class MessageAllFreezeInRadius : public Message
{
public:
    MessageAllFreezeInRadius(float radius, const sf::Vector2f& centerPos) :
        mRadius(radius),
        mCenterPos(centerPos)
    {}

public:
    float mRadius = 0.f;
    sf::Vector2f mCenterPos;
    float mSpeed;
};

class MessageSetSpeedInRadius : public Message 
{
public:
    MessageSetSpeedInRadius(float radius, const sf::Vector2f& centerPos, float speed) :
        mRadius(radius),
        mCenterPos(centerPos),
        mSpeed(speed)
    {}

public:
    float mRadius = 0.f;
    sf::Vector2f mCenterPos;
    float mSpeed = 0.f;
};