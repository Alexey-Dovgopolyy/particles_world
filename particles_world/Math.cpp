#include "Math.h"

#include <cmath>
#include "Math.h"

sf::Vector2f Math::getPerp(const sf::Vector2f& vec)
{
    sf::Vector2f result;
    result.x = -vec.y;
    result.y = vec.x;
    return result;
}

float Math::dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    float dotProd = v1.x * v2.x + v1.y * v2.y;
    return dotProd;
}

float Math::vectorLength(const sf::Vector2f& vec)
{
    float result = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
    return result;
}

sf::Vector2f Math::normalize(const sf::Vector2f& vec)
{
    sf::Vector2f result(vec);
    normalizeThis(result);
    return result;
}

void Math::normalizeThis(sf::Vector2f& vec)
{
    float vecLength = vectorLength(vec);
    vec.x = vec.x / vecLength;
    vec.y = vec.y / vecLength;
}

float Math::distanceFromPointToSegment(const sf::Vector2f& point, const sf::Vector2f& segmentP1, const sf::Vector2f& segmentP2)
{
    float A = segmentP1.y - segmentP2.y;
    float B = segmentP2.x - segmentP1.x;
    float C = (segmentP1.x * segmentP2.y) - (segmentP2.x * segmentP1.y);

    float distance = (std::fabs(A * point.x + B * point.y + C)) / (std::sqrt(A * A + B * B));
    return distance;
}
