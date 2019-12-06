#include "QuadTree.h"

#include "ServiceProvider.h"


QuadTree::QuadTree()
{
    mDebugRectangle.setFillColor(sf::Color::Transparent);
    mDebugRectangle.setOutlineColor(sf::Color::Red);
    mDebugRectangle.setOutlineThickness(0.5f);
}

QuadTree::~QuadTree()
{
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

void QuadTree::clear()
{
    for (std::unique_ptr<QuadTree>& child : mChildren)
    {
        child->clear();
    }

    mChildren.clear();
    mObjects.clear();
}

void QuadTree::split()
{
    bool canSplit = (mObjects.size() > mMaxObjects && mChildren.empty());
    if (canSplit)
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
}

std::vector<int> QuadTree::getIndices(Particle* particle)
{
    std::vector<int> result;

    sf::FloatRect nodeBounds = particle->getBoundingRect();

    //for (std::unique_ptr<QuadTree>& quad : mChildren)
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
    if (mChildren.empty())
    {
        mObjects.insert(particle);

        if (mObjects.size() > mMaxObjects && mLevel < mMaxLevel)
        {
            split();

            for (Particle* object : mObjects)
            {
                std::vector<int> quadIndices = getIndices(object);

                for (int index : quadIndices)
                {
                    mChildren[index]->insert(object);
                }
            }

            mObjects.clear();
        }
    }
    else
    {
        std::vector<int> quadIndices = getIndices(particle);

        for (int index : quadIndices)
        {
            mChildren[index]->insert(particle);
        }
    }
}

void QuadTree::retrieve(std::set<Particle*>& possibleCollisions, Particle* particle)
{
    PhysicsService* physics = ServiceProvider::getPhysicsService();

    if (mChildren.empty())
    {
        for (Particle* object : mObjects)
        {
            if (object != particle)
            {
                //possibleCollisions.insert(object);

                bool interact = physics->isInteract(particle, object);
                if (interact)
                {
                    possibleCollisions.insert(object);
                }
            }
        }
    }
    else
    {
        std::vector<int> entityQuads = getIndices(particle);

        for (int quadIndex : entityQuads)
        {
            mChildren[quadIndex]->retrieve(possibleCollisions, particle);
        }
    }
}

void QuadTree::drawCurrent()
{
//     sf::RectangleShape debugRectangle;
//     debugRectangle.setFillColor(sf::Color::Yellow);
//     debugRectangle.setOutlineColor(sf::Color::Red);
//     debugRectangle.setOutlineThickness(2.f);
//     debugRectangle.setPosition(sf::Vector2f(mBounds.left, mBounds.top));

    sf::RenderWindow* window = ServiceProvider::getWindowService()->getWindow();
    window->draw(mDebugRectangle);

    for (auto& child : mChildren)
    {
        child->drawCurrent();
    }
}
