#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct ColorParticle {
    glm::vec3 pos;
    float size;
    glm::u8vec4 color;
};

struct ParticleData {
    glm::vec3 speed;
    float life;
};
