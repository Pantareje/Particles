#pragma once

// language=glsl
constexpr auto PARTICLE_VS = static_cast<const char*>(R"(
    #version 330 core

    layout (location = 0) in vec3 vert;
    layout (location = 1) in vec3 center;
    layout (location = 2) in float size;
    layout (location = 3) in vec4 color;

    uniform vec3 cameraRight;
    uniform vec3 cameraUp;

    uniform mat4 viewProjection;

    out vec4 ParticleColor;

    void main()
    {
        float scale = 10.0f;
        ParticleColor = color;
        vec3 vertexPos = center + size * (cameraRight * vert.x + cameraUp * vert.y);
        gl_Position = viewProjection * vec4(vertexPos, 1.0f);
    }
)");

// language=glsl
constexpr auto PARTICLE_FS = static_cast<const char*>(R"(
    #version 330 core

    in vec4 ParticleColor;

    out vec4 color;

    void main()
    {
        color = ParticleColor;
    }
)");
