#pragma once

#include <cstdint>

struct ApplicationConfiguration {
    size_t particleCount = 10000;
    uint16_t width = 800;
    uint16_t height = 800;
    const char* name = "Partículas";
};
