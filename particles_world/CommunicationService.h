#pragma once

#include "MessageListener.h"

#include <map>
#include <set>
#include <string>
#include <queue>

class CommunicationService
{
    friend class ServiceProvider;

public:
    void addListener(MessageType messageType, MessageListener* listener);
    void removeListener(MessageType messageType, MessageListener* listener);

    void queueMessage(MessageType messageType, Message* message);
    void executeAll();
    void executeOne();

private:
    CommunicationService();
    CommunicationService(const CommunicationService& window) = delete;

    bool init();

    void execute(MessageType messageType, Message* message);

    static void create();
    static void cleanup();
    static CommunicationService* getInstance();

private:
    std::map<MessageType, std::set<MessageListener*>> mListeners;
    //std::multimap<MessageType, MessageListener> mListeners;
    std::queue<std::pair<MessageType, Message*>> mMessagesQueue;

    static CommunicationService* sInstance;
};

