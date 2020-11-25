#include "ImageAnalyser.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

int main(int argc, char const* argv[]) {
    auto&                 analyser = ImageAnalyser::Get();
    std::filesystem::path path("test2.png");
    auto                  fileSize = std::filesystem::file_size(path);
    std::vector<uint8_t>  buffer(fileSize);

    std::ifstream ifs(path, std::ios::binary);
    ifs.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    ifs.close();

    analyser.UpdateDesk(buffer);
    std::cout << "============= Desk ================" << std::endl;
    auto deskPos = analyser.GetDeskPosition();
    std::cout << "(" << deskPos.x << "," << deskPos.y << ")" << std::endl;
    auto deskSize = analyser.GetDeskSize();
    std::cout << "(" << deskSize.x << "," << deskSize.y << ")" << std::endl;

    std::cout << "============= Balls ================" << std::endl;
    auto balls = analyser.GetColorBallsPosition();
    std::cout << "Find " << balls.size() << " balls" << std::endl;
    for (auto&& b : balls)
        std::cout << "(" << b.x << "," << b.y << ")" << std::endl;

    std::cout << "============= Holes ================" << std::endl;
    auto holes = analyser.GetHolesPosition();
    for (auto&& h : holes) {
        h = h - deskPos;
        std::cout << "(" << h.x << "," << h.y << ")" << std::endl;
    }

    return 0;
}
