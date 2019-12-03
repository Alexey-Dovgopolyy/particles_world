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

    void setBounds(const sf::FloatRect& bounds);
    void setLevel(int level);

    void clear();
    void split();
    std::vector<int> getIndices(Particle* particle);
    void insert(Particle* particle);
    void retrieve(std::set<Particle*>& possibleCollisions, Particle* particle);

    void drawCurrent();

private:
    int mMaxLevel = 5;
    int mMaxObjects = 5;
    std::vector<std::unique_ptr<QuadTree>> mChildren;
    std::set<Particle*> mObjects;
    sf::FloatRect mBounds;
    int mLevel = 0;

    sf::RectangleShape mDebugRectangle;
};

