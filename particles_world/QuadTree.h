#pragma once

#include "Particle.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>
#include <set>
#include <memory>

class QuadTree
{
public:
    QuadTree();
    ~QuadTree();

    void init();

    void setBounds(const sf::FloatRect& bounds);
    void setLevel(int level);
    bool intesects(const sf::FloatRect& bounds);

    void clear();
    void split();
    std::vector<int> getIndices(Particle* particle);
    void insert(Particle* particle);
    void retrieve(std::vector<std::vector<Particle*>>& possibleCollisions);

    void draw();

private:
    int mMaxLevel = 5;
    int mMaxObjects = 7;
    std::vector<std::unique_ptr<QuadTree>> mChildren;
    std::vector<Particle*> mObjects;
    sf::FloatRect mBounds;
    int mLevel = 0;
    //bool mIsExists = false;
    bool mHasChildren = false;

    sf::RectangleShape mDebugRectangle;
};

