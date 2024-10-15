#include "Application.h"

namespace {
    int Main() {
        constexpr ApplicationConfiguration config = {
            .particleCount = 150'000
        };

        Application app(config);
        return app.Run();
    }
}

#ifdef WIN32

#include <Windows.h>

int WINAPI wWinMain(
    [[maybe_unused]] _In_ HINSTANCE hInstance,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
    [[maybe_unused]] _In_ PWSTR lpCmdLine,
    [[maybe_unused]] _In_ int nShowCmd
) {
    return Main();
}

#else

int main() {
    return Main();
}

#endif
