#pragma once


class InputService
{
    friend class ServiceProvider;

public:
    void processInput();

private:
    InputService();
    InputService(const InputService& window) = delete;

    bool init();

    static void create();
    static void cleanup();
    static InputService* getInstance();

private:
    static InputService* sInstance;

    bool mIsMouseInWindow = true;
};

