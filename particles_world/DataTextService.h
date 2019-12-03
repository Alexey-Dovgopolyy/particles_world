#pragma once

class DataTextService
{
    friend class ServiceProvider;

public:


private:
    DataTextService();
    DataTextService(const DataTextService& window) = delete;

    bool init();

    static void create();
    static void cleanup();
    static DataTextService* getInstance();

private:

    static DataTextService* sInstance;
};
