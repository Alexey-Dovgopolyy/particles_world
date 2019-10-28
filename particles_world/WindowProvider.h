#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class WindowProvider
{
    friend class ServiceProvider;

public:
    sf::RenderWindow* getWindow();

private:
    WindowProvider();
    WindowProvider(const WindowProvider& window) = delete;

    void init();

    static void create();
    static void cleanup();
    static WindowProvider* getInstance();

private:
    std::unique_ptr<sf::RenderWindow> mWindow;

    static WindowProvider* sInstance;
};

