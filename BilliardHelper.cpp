#include "BilliardHelper.hpp"

#include <algorithm>
#include <stdexcept>
#include <limits>
#include <iostream>

void BilliardHelper::UpdateBalls(const std::vector<Vec2>& ballsPosition) {
    if (ballsPosition.empty()) return;
    m_CueBall = ballsPosition.back();

    m_ColoredBalls.clear();
    m_ColoredBalls.insert(m_ColoredBalls.end(), ballsPosition.begin(), ballsPosition.end() - 1);
}

std::array<Vec2, 2> BilliardHelper::GetHittingPoint(const Vec2& cursor) const {
    auto pow2 = [](double x) { return x * x; };

    auto ball = FindBall(cursor);

    size_t targetHole   = 0;
    double minDistance2 = std::numeric_limits<double>::max();
    for (size_t i = 0; i < m_Holes.size(); i++) {
        Vec2 a = m_Holes[i] - ball, b = cursor - ball;
        if (dot(a, b) > 0) continue;
        double distance2 = dot(b, b) - pow2(dot(a, b) / a.norm());
        if (distance2 < minDistance2) {
            minDistance2 = distance2;
            targetHole   = i;
        }
    }

    return GetHittingPoint(cursor, m_Holes[targetHole]);
}

std::array<Vec2, 2> BilliardHelper::GetHittingPoint(const Vec2& cursor, const Vec2& targetPoint) const {
    auto ball = FindBall(cursor);
    if (!ball)
        return {ball, targetPoint};

    auto unitDir = ball - targetPoint;
    unitDir      = unitDir / unitDir.norm();

    auto result = 2 * m_BallRadius * unitDir + ball;
    if (result.x < m_BallRadius || result.x > m_DeskSize.x - m_BallRadius || result.y < m_BallRadius || result.y > m_DeskSize.y - m_BallRadius)
        result = {-1, -1};

    return {result, targetPoint};
}

std::vector<Vec2> BilliardHelper::GetReflectionPath(const Vec2& cursor, unsigned reflectNum = 1) const {
    Vec2 currDir = cursor - m_CueBall;

    if (currDir.x == 0 || currDir.y == 0) return {};

    std::vector<Vec2> result;
    Vec2              cueBall = m_CueBall;
    while (reflectNum != 0) {
        Vec2 pos;
        if (currDir.x > 0) {
            pos.x = m_InnerRight;
            pos.y = cueBall.y + (m_InnerRight - cueBall.x) / currDir.x * currDir.y;
        } else if (currDir.x < 0) {
            pos.x = m_InnerLeft;
            pos.y = cueBall.y - (cueBall.x - m_InnerLeft) / currDir.x * currDir.y;
        }

        if (pos.y > m_InnerBottom) {
            pos.y = m_InnerBottom;
            pos.x = cueBall.x + (m_InnerBottom - cueBall.y) / currDir.y * currDir.x;
        } else if (pos.y < m_InnerTop) {
            pos.y = m_InnerTop;
            pos.x = cueBall.x - (cueBall.y - m_InnerTop) / currDir.y * currDir.x;
        }
        result.emplace_back(pos);
        cueBall = pos;

        if (pos.x == m_InnerLeft || pos.x == m_InnerRight)
            currDir.x = -currDir.x;
        else if (pos.y == m_InnerTop || pos.y == m_InnerBottom)
            currDir.y = -currDir.y;
        else
            throw std::logic_error("reflect point is not on the border");

        reflectNum--;
    }

    return result;
}

Vec2 BilliardHelper::FindBall(const Vec2& cursor) const {
    auto ball = std::find_if(std::begin(m_ColoredBalls), std::end(m_ColoredBalls), [&](const Vec2& b) {
        return (cursor - b).norm() <= 2 * m_BallRadius + 3;
    });
    return ball != std::end(m_ColoredBalls) ? *ball : Vec2{-1, -1};
}
