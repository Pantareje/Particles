#pragma once

#include "Math.h"
#include "Camera.h"
#include "Particle.h"
#include "CpuParticlePool.h"
#include "ApplicationConfiguration.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random>

class Application final {
    std::mt19937 m_mt { std::random_device()() };
    std::uniform_real_distribution<float> m_dist { 0.0f, 1.0f };

    GLFWwindow* m_window { nullptr };

    Camera m_camera {
        glm::vec3(0.0f, 0.0f, 2.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        PI_DIV4,
        1.0f,
        0.1f, 100.0f
    };

    CpuParticlePool<ColorParticle, ParticleData> m_particles;

    GLint m_cameraRightLocation { 0 };
    GLint m_cameraUpLocation { 0 };
    GLint m_viewProjectionLocation { 0 };

    float m_cameraDistance = 3.5f;
    float m_cameraHAngle = 0.0f;
    float m_cameraVAngle = PI_DIV2;

    bool m_particlesUpdated { true };
    bool m_cameraUpdated { true };

    void InitWindow(uint16_t width, uint16_t height, const char* name);
    void InitParticles(size_t particleCount);
    void InitShaders();
    void InitOpenGlBuffers();

    [[nodiscard]] ColorParticle CreateParticle();
    ParticleData CreateParticleData(float lifeOffset);
    void SetCameraLocation(float distance, float hAngle, float vAngle);

    void BeforeRender(float deltaTime);
    void Render() const;
    void AfterRender(float deltaTime);

    void UpdateParticles(float deltaTime);
    void UpdatePosition(float deltaTime);

    void UploadParticles();
    void UploadPosition();

public:
    Application();

    int Run(const ApplicationConfiguration& config);

private:
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
