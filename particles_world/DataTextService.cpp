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
    window->draw(mInitSpeed);
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

void DataTextService::setInitialSpeed(float initSpeed)
{
    std::string initSpeedStr = "Initial speed: " + std::to_string(initSpeed);
    mInitSpeed.setString(initSpeedStr);
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

    std::vector<sf::Text*> texts;
    texts.push_back(&mFps);
    texts.push_back(&mParticlesCount);
    texts.push_back(&mUpdateTime);
    texts.push_back(&mInitSpeed);
    texts.push_back(&mEnergy);

    for (sf::Text* text : texts)
    {
        text->setFont(mFont);
        text->setCharacterSize(12);
    }

    float textPosStep = 12.f;
    float textPosY = 10.f;

    for (sf::Text* text : texts)
    {
        text->setPosition(10.f, textPosY);
        textPosY += textPosStep;
    }

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
