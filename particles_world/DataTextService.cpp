#include "DataTextService.h"
#include "ServiceProvider.h"
#include "WorldService.h"
#include "WindowService.h"
#include "ConfigService.h"

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

    World* world = ServiceProvider::getWorldService()->getWorld();

    size_t count = world->getParticlesCount();
    std::string countStr = "Count " + std::to_string(count);
    mParticlesCount.setString(countStr);

    float averageSpeed = world->getAverageSpeed();
    std::string averageSpeedStr = "Average speed: " + std::to_string(averageSpeed);
    mEnergy.setString(averageSpeedStr);

    int updateTimes = world->getUpdateTimes();
    std::string times = "Updates per frame: " + std::to_string(updateTimes);
    mUpdateTimes.setString(times);
}

void DataTextService::draw()
{
    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();

    window->draw(mFps);
    window->draw(mParticlesCount);
    window->draw(mUpdateTime);
    window->draw(mInitSpeed);
    window->draw(mEnergy);
    window->draw(mGravity);
    window->draw(mUpdateTimes);

    window->draw(mIncSpeed);
    window->draw(mDecSpeed);
    window->draw(mFreeze);
    window->draw(mQuadTree);
    window->draw(mIncDecTimes);
    window->draw(mCreateHeatMode);
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
    texts.push_back(&mUpdateTimes);

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

    mIncSpeed.setString("Inc speed by 10% (Q)");
    mDecSpeed.setString("Dec speed by 10% (W)");
    mFreeze.setString("Freeze all particles (E)");
    mQuadTree.setString("On/Off quad tree view (R)");
    mIncDecTimes.setString("Dec/Inc updates per frame (A/S)");
    mCreateHeatMode.setString("Switch creat/heat mode (D)");

    texts.clear();
    texts.push_back(&mIncSpeed);
    texts.push_back(&mDecSpeed);
    texts.push_back(&mFreeze);
    texts.push_back(&mQuadTree);
    texts.push_back(&mIncDecTimes);
    texts.push_back(&mCreateHeatMode);

    for (sf::Text* text : texts)
    {
        text->setFont(mFont);
        text->setCharacterSize(12);
        float textWidth = text->getGlobalBounds().width;
        text->setOrigin(textWidth, 0.f);
    }

    textPosY = 10.f;

    float windowWidth = static_cast<float>(ServiceProvider::getConfigService()->getWinSizeX());
    float textPosX = windowWidth - 10.f;

    for (sf::Text* text : texts)
    {
        text->setPosition(textPosX, textPosY);
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
