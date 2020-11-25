#include "ImageAnalyser.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <limits>

void ImageAnalyser::UpdateDesk(const std::vector<uint8_t>& desk) {
    if (desk.empty()) return;

    m_ImgMat = cv::imdecode(desk, cv::IMREAD_COLOR);
    if (!m_Gray.has_value()) m_Gray = cv::Mat{};

    auto& mat  = std::any_cast<cv::Mat&>(m_ImgMat);
    auto& gray = std::any_cast<cv::Mat&>(m_Gray);

    cv::Mat m1, m2;
    cv::cvtColor(mat, m1, cv::COLOR_BGR2HLS);
    cv::Scalar lower(49, 0, 0);
    cv::Scalar upperb(59, 255, 255);
    cv::inRange(m1, lower, upperb, gray);
}

double ImageAnalyser::GetBallSize() { return 13; }

std::vector<Vec2> ImageAnalyser::GetBallsPosition() {
    auto& mat  = std::any_cast<cv::Mat&>(m_ImgMat);
    auto& gray = std::any_cast<cv::Mat&>(m_Gray);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     20,              // change this value to detect circles with different distances to each other
                     100, 14, 13, 13  // change the last two parameters
                                      // (min_radius & max_radius) to detect larger circles
    );

    auto pow2 = [](auto x) { return x * x; };

    std::vector<Vec2> balls;
    balls.reserve(circles.size());
    Vec2 cueBall = {-1, -1};
    for (size_t i = 0; i < circles.size(); i++) {
        auto     center = Vec2{circles[i][0] + 0.5, circles[i][1] + 0.5};
        cv::Rect rect(center.x - 9, center.y - 9, 18, 18);
        auto     color    = cv::mean(mat(rect));
        auto     distance = std::sqrt(pow2(200 - color[0]) + pow2(200 - color[1]) + pow2(200 - color[2]));
        if (distance < 50) {
            cueBall = center;
        } else
            balls.emplace_back(center);
    }
    if (cueBall)
        balls.emplace_back(cueBall);

    return balls;
}
