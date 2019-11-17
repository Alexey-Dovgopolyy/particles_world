#pragma once

#include <SFML/System/Vector2.hpp>

class Math
{
public:
    static sf::Vector2f getPerp(const sf::Vector2f& vec);
    static float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);
    static float vectorLength(const sf::Vector2f& vec);
    static float vectorLengthQuad(const sf::Vector2f& vec);
    static sf::Vector2f normalize(const sf::Vector2f& vec);
    static void normalizeThis(sf::Vector2f& vec);
    static float distanceFromPointToSegment(const sf::Vector2f& point, const sf::Vector2f& segmentP1, const sf::Vector2f& segmentP2);
    static int randomInt(int from, int to);
};

