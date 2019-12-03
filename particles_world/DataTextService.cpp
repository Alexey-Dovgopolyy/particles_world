#include "DataTextService.h"

DataTextService* DataTextService::sInstance = nullptr;

DataTextService::DataTextService()
{

}

bool DataTextService::init()
{
    return true;
}

void DataTextService::create()
{
    sInstance = new DataTextService();
}

void DataTextService::cleanup()
{
    delete sInstance;
    sInstance = nullptr;
}

DataTextService* DataTextService::getInstance()
{
    return sInstance;
}
