#pragma once

#include "MessageListener.h"


#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class WindowService
    : public MessageListener
{
    friend class ServiceProvider;

public:
    sf::RenderWindow* getWindow();

    void handleMessage(MessageType messageType, Message* message) override;

private:
    WindowService();
    WindowService(const WindowService& window) = delete;

    bool init();

    static void create();
    static void cleanup();
    static WindowService* getInstance();

private:
    std::unique_ptr<sf::RenderWindow> mWindow;

    static WindowService* sInstance;
};

