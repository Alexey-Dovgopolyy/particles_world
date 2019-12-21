#include "QuadTree.h"

#include "ServiceProvider.h"
#include "WindowService.h"

QuadTree::QuadTree()
{
    mDebugRectangle.setFillColor(sf::Color::Transparent);
    mDebugRectangle.setOutlineColor(sf::Color::Red);
    mDebugRectangle.setOutlineThickness(0.5f);

    mObjects.reserve(mMaxObjects);
}

QuadTree::~QuadTree()
{
}

void QuadTree::init()
{
    if (mLevel == mMaxLevel)
    {
        return;
    }

    split();

    for (std::unique_ptr<QuadTree>& child : mChildren)
    {
        child->init();
    }
}

void QuadTree::setBounds(const sf::FloatRect& bounds)
{
    mBounds = bounds;

    mDebugRectangle.setPosition(sf::Vector2f(mBounds.left, mBounds.top));
    mDebugRectangle.setSize(sf::Vector2f(mBounds.width, mBounds.height));
}

void QuadTree::setLevel(int level)
{
    mLevel = level;
}

bool QuadTree::intesects(const sf::FloatRect& bounds)
{
    return mBounds.intersects(bounds);
}

void QuadTree::clear()
{
    for (std::unique_ptr<QuadTree>& child : mChildren)
    {
        child->clear();
    }

    mObjects.clear();
    mHasChildren = false;
}

void QuadTree::split()
{
    float boundsMidX = mBounds.left + mBounds.width / 2.f;
    float boundsMidY = mBounds.top + mBounds.height / 2.f;

    float quadWidth = mBounds.width / 2.f;
    float quadHeight = mBounds.height / 2.f;

    sf::FloatRect rectQuad1(boundsMidX, mBounds.top, quadWidth, quadHeight);
    sf::FloatRect rectQuad2(mBounds.left, mBounds.top, quadWidth, quadHeight);
    sf::FloatRect rectQuad3(mBounds.left, boundsMidY, quadWidth, quadHeight);
    sf::FloatRect rectQuad4(boundsMidX, boundsMidY, quadWidth, quadHeight);

    std::unique_ptr<QuadTree> quadTree1 = std::make_unique<QuadTree>();
    std::unique_ptr<QuadTree> quadTree2 = std::make_unique<QuadTree>();
    std::unique_ptr<QuadTree> quadTree3 = std::make_unique<QuadTree>();
    std::unique_ptr<QuadTree> quadTree4 = std::make_unique<QuadTree>();

    int level = (mLevel + 1);

    quadTree1->setBounds(rectQuad1);
    quadTree2->setBounds(rectQuad2);
    quadTree3->setBounds(rectQuad3);
    quadTree4->setBounds(rectQuad4);

    quadTree1->setLevel(level);
    quadTree2->setLevel(level);
    quadTree3->setLevel(level);
    quadTree4->setLevel(level);

    mChildren.push_back(std::move(quadTree1));
    mChildren.push_back(std::move(quadTree2));
    mChildren.push_back(std::move(quadTree3));
    mChildren.push_back(std::move(quadTree4));
}

std::vector<int> QuadTree::getIndices(Particle* particle)
{
    std::vector<int> result;

    sf::FloatRect nodeBounds = particle->getBoundingRect();

    for (size_t i = 0; i < mChildren.size(); i++)
    {
        QuadTree* quad = mChildren[i].get();

        bool isIntersectsWithQuad = quad->mBounds.intersects(nodeBounds);
        if (isIntersectsWithQuad)
        {
            result.push_back(i);
        }
    }

    return result;
}

void QuadTree::insert(Particle* particle)
{
    if (mHasChildren)
    {
        const sf::FloatRect& particleBounds = particle->getBoundingRect();

        if (mChildren[0]->intesects(particleBounds)) mChildren[0]->insert(particle);
        if (mChildren[1]->intesects(particleBounds)) mChildren[1]->insert(particle);
        if (mChildren[2]->intesects(particleBounds)) mChildren[2]->insert(particle);
        if (mChildren[3]->intesects(particleBounds)) mChildren[3]->insert(particle);
    }
    else
    {
        mObjects.push_back(particle);

        bool needUseLowerLevel = (mObjects.size() > mMaxObjects && mLevel < mMaxLevel);
        if (needUseLowerLevel)
        {
            mHasChildren = true;

            for (Particle* object : mObjects)
            {
                insert(object);
            }

            mObjects.clear();
        }
    }
}

void QuadTree::retrieve(std::vector<std::vector<Particle*>>& possibleCollisions)
{
    if (!mHasChildren && mObjects.size() > size_t(1))
    {
        possibleCollisions.push_back(mObjects);
    }
    else
    {
        for (std::unique_ptr<QuadTree>& child : mChildren)
        {
            child->retrieve(possibleCollisions);
        }
    }
}

void QuadTree::draw()
{
    if (mHasChildren == false)
    {
        sf::RectangleShape debugRectangle;
        debugRectangle.setFillColor(sf::Color::Yellow);
        debugRectangle.setOutlineColor(sf::Color::Red);
        debugRectangle.setOutlineThickness(2.f);
        debugRectangle.setPosition(sf::Vector2f(mBounds.left, mBounds.top));

        sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
        window->draw(mDebugRectangle);
    }
    else
    {
        for (auto& child : mChildren)
        {
            child->draw();
        }
    }
}
