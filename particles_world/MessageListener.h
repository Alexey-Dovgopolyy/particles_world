#pragma once

#include "Messages.h"

enum class MessageType
{
    closeWindow,
    mouseDown,
    mouseMoved,
    mouseUp,
    mouseWheelMoved,
    spawnParticle,
    incInitialSpeed,
    decInitialSpeed,
    incAllSpeed,
    decAllSpeed,
    allFreeze
};

class MessageListener
{
public:
    virtual void handleMessage(MessageType messageType, Message* message) = 0;
};