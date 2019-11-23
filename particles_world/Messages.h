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