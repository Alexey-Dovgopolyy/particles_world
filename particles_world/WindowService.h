#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class WindowService
{
    friend class ServiceProvider;

public:
    sf::RenderWindow* getWindow();

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

