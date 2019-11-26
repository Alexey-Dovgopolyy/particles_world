#pragma once

#include "Messages.h"

enum class MessageType
{
    closeWindow,
    mouseDown,
    mouseMoved,
    mouseUp,
    mouseWheelMoved,
    spawnParticle
};

class MessageListener
{
public:
    virtual void handleMessage(MessageType messageType, Message* message) = 0;
};