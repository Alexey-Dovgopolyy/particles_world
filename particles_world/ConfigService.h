#pragma once
class ConfigService
{
    friend class ServiceProvider;

public:


private:
    ConfigService();
    ConfigService(const ConfigService& config) = delete;

    bool init();

    static void create();
    static void cleanup();
    static ConfigService* getInstance();

private:
    static ConfigService* sInstance;

};

