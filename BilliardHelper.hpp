#pragma once

#include "Vector.hpp"

#include <vector>
#include <array>
#include <cmath>

class BilliardHelper {
public:
    void UpdateBalls(const std::vector<Vec2>& ballsPosition);

    // Get the hitting point that can let the ball into the hole
    std::array<Vec2, 2> GetHittingPoint(const Vec2& cursor) const;
    // Get the hitting point that can let the ball pass the target point.
    std::array<Vec2, 2> GetHittingPoint(const Vec2& cursor, const Vec2& targetPoint) const;
    // Get the reflection path
    std::vector<Vec2> GetReflectionPath(const Vec2& cursor, unsigned reflectNum) const;

private:
    Vec2 FindBall(const Vec2& cursor) const;

    Vec2 m_DeskSize = {1024, 738};

    double            m_BallRadius = 13.0;
    Vec2              m_CueBall;
    std::vector<Vec2> m_ColoredBalls;

    std::array<Vec2, 6> m_Holes = {
        Vec2{46, 68},
        Vec2{512, 57},
        Vec2{980, 68},
        Vec2{46, 542},
        Vec2{512, 553},
        Vec2{980, 542}};

    double m_InnerTop    = 65.0;
    double m_InnerLeft   = 44.5;
    double m_InnerBottom = 543.5;
    double m_InnerRight  = m_DeskSize.x - m_InnerLeft;
};
