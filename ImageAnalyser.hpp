#pragma once
#include "Vector.hpp"

#include <vector>
#include <array>
#include <any>

class ImageAnalyser {
public:
    static ImageAnalyser& Get() {
        static ImageAnalyser analyser;
        return analyser;
    }
    ImageAnalyser(const ImageAnalyser&) = delete;
    ImageAnalyser(ImageAnalyser&&)      = delete;
    ImageAnalyser& operator=(const ImageAnalyser&) = delete;
    ImageAnalyser& operator=(ImageAnalyser&&) = delete;

    void UpdateDesk(const std::vector<uint8_t>& desk);

    // the last ball is cue ball
    std::vector<Vec2> GetBallsPosition();

private:
    ImageAnalyser()  = default;
    ~ImageAnalyser() = default;

    std::any m_ImgMat;
    std::any m_Gray;
};