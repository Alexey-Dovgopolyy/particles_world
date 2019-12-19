#include "DataTextService.h"
#include "ServiceProvider.h"
#include "WorldService.h"
#include "WindowService.h"

DataTextService* DataTextService::sInstance = nullptr;

void DataTextService::update(/*float dt*/)
{
    static int fps = 0;
    static sf::Time fpsTime = sf::Time::Zero;
    static sf::Clock fpsClock;

    fpsTime += fpsClock.restart();

    if (fpsTime >= sf::seconds(1.f))
    {
        setFps(fps);
        fpsTime -= sf::seconds(1.f);
        fps = 0;
    }

    fps++;

    size_t count = ServiceProvider::getWorldService()->getWorld()->getParticlesCount();
    std::string countStr = "Count " + std::to_string(count);
    mParticlesCount.setString(countStr);
}

void DataTextService::draw()
{
    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();

    window->draw(mFps);
    window->draw(mParticlesCount);
    window->draw(mUpdateTime);
}

void DataTextService::setFps(int fps)
{
    std::string fpsStr = "FPS: " + std::to_string(fps);
    mFps.setString(fpsStr);
}

void DataTextService::setUpdateTime(float time)
{
    std::string updateStr = "Update: " + std::to_string(time);
    mUpdateTime.setString(updateStr);
}

DataTextService::DataTextService()
{

}

bool DataTextService::init()
{
    if (!mFont.loadFromFile("Sansation.ttf"))
    {
        return false;
    }

    mFps.setFont(mFont);
    mParticlesCount.setFont(mFont);
    mUpdateTime.setFont(mFont);

    mFps.setCharacterSize(12);
    mParticlesCount.setCharacterSize(12);
    mUpdateTime.setCharacterSize(12);

    mFps.setPosition(10.f, 10.f);
    mParticlesCount.setPosition(10.f, 24.f);
    mUpdateTime.setPosition(10.f, 38.f);

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
