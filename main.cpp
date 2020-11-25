#include "BilliardHelper.hpp"
#include "Application.hpp"
#include "ImageAnalyser.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Application app;
    if (int ret = app.Initialize(); ret != 0) return 1;

    auto&          anaylser = ImageAnalyser::Get();
    BilliardHelper helper;

    app.AddCallback(1, [&]() -> void {
           anaylser.UpdateDesk(app.GetDeskImage());
           helper.UpdateBalls(anaylser.GetBallsPosition());

           auto [hitpoint, target] = helper.GetHittingPoint(app.GetCursor());
           app.MoveCursor(hitpoint).DrawLine(hitpoint, target);
       })
        .AddCallback(2, [&]() -> void {
            anaylser.UpdateDesk(app.GetDeskImage());
            helper.UpdateBalls(anaylser.GetBallsPosition());

            auto hintpoints = helper.GetReflectionPath(app.GetCursor(), 4);
            app.DrawLines(hintpoints);
        });

    while (!app.Quit()) {
        app.Tick();
    }
    return 0;
}
