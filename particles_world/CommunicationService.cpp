#include "CommunicationService.h"

CommunicationService* CommunicationService::sInstance = nullptr;

void CommunicationService::addListener(MessageType messageType, MessageListener* listener)
{
    mListeners[messageType].insert(listener);
}

void CommunicationService::removeListener(MessageType messageType, MessageListener* listener)
{
    auto it = mListeners.find(messageType);
    if (it != mListeners.end())
    {
        auto listenerIt = it->second.find(listener);
        it->second.erase(listenerIt);
    }
}

void CommunicationService::queueMessage(MessageType messageType, Message* message)
{
    mMessagesQueue.push({ messageType, message });
}

void CommunicationService::executeAll()
{
    while (!mMessagesQueue.empty())
    {
        std::pair<MessageType, Message*>& message = mMessagesQueue.front();
        execute(message.first, message.second);
        mMessagesQueue.pop();
        delete message.second;
    }
}

void CommunicationService::executeOne()
{
    std::pair<MessageType, Message*>& message = mMessagesQueue.front();
    execute(message.first, message.second);
    mMessagesQueue.pop();
    delete message.second;
}

CommunicationService::CommunicationService()
{

}

bool CommunicationService::init()
{
    return true;
}

void CommunicationService::execute(MessageType messageType, Message* message)
{
    auto it = mListeners.find(messageType);
    if (it != mListeners.end())
    {
        std::set<MessageListener*>& listenersForMessage = it->second;
        for (MessageListener* listener : listenersForMessage)
        {
            listener->handleMessage(messageType, message);
        }
    }
}

void CommunicationService::create()
{
    sInstance = new CommunicationService();
}

void CommunicationService::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

CommunicationService* CommunicationService::getInstance()
{
    return sInstance;
}
