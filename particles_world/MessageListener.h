#pragma once

#include "Messages.h"
#include <functional>
#include <map>

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
    allFreeze,
    allFreezeInRad,
    setSpeedInRad,
    switchQuadTree,
    incTime,
    decTime,
    switchCreateHeat,

    incAttraction,
    decAttraction,

    incRepelling,
    decRepelling
};

class MessageListener
{
public:
    virtual void handleMessage(MessageType messageType, Message* message)
    {
		auto it = mHandlers.find(messageType);
		if (it != mHandlers.end())
		{
			it->second(message);
		}
    }

protected:
    std::map<MessageType, std::function<void(Message*)>> mHandlers;
};