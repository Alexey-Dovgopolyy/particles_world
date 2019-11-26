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