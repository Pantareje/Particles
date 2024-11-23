#include "Application.h"
#include "CpuParticlePool.h"
#include "ParticleShaders.h"

#include <algorithm>
#include <iostream>

namespace {
    [[maybe_unused]] void GLAPIENTRY MessageCallback(
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei length,
        [[maybe_unused]] const GLchar* message,
        [[maybe_unused]] const void* userParam
    ) {
        std::cerr << message << std::endl;
    }
}


void Application::InitWindow(uint16_t width, uint16_t height, const char* name) {
    int res;

    res = ::glfwInit();
    if (!res) throw std::exception();

    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    ::glfwWindowHint(GLFW_DEPTH_BITS, 24);

    m_window = ::glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (!m_window) throw std::exception();

    ::glfwSetWindowUserPointer(m_window, this);

    ::glfwMakeContextCurrent(m_window);
    ::glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

    res = ::gladLoadGLLoader(reinterpret_cast<GLADloadproc>(::glfwGetProcAddress));
    if (!res) throw std::exception();

#ifndef NDEBUG
    ::glEnable(GL_DEBUG_OUTPUT);
    ::glDebugMessageCallback(MessageCallback, nullptr);
#endif

    ::glEnable(GL_CULL_FACE);
    ::glEnable(GL_DEPTH_TEST);
    ::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    ::glClearDepth(1.0f);
}

void Application::InitParticles(size_t particleCount) {
    m_particles = CpuParticlePool<ColorParticle, ParticleData>(particleCount);

    for (size_t i = 0; i < m_particles.GetMaxCount(); i++) {
        m_particles.AddParticle(CreateParticle(), CreateParticleData(0.0f));
    }
}

void Application::InitShaders() {
    constexpr const char* vertexShaderSources[] = { PARTICLE_VS };
    const unsigned int vertexShader = ::glCreateShader(GL_VERTEX_SHADER);
    ::glShaderSource(vertexShader, 1, vertexShaderSources, nullptr);
    ::glCompileShader(vertexShader);

    constexpr const char* fragmentShaderSources[] = { PARTICLE_FS };
    const unsigned int fragmentShader = ::glCreateShader(GL_FRAGMENT_SHADER);
    ::glShaderSource(fragmentShader, 1, fragmentShaderSources, nullptr);
    ::glCompileShader(fragmentShader);

    const unsigned int shaderProgram = ::glCreateProgram();
    ::glAttachShader(shaderProgram, vertexShader);
    ::glAttachShader(shaderProgram, fragmentShader);
    ::glLinkProgram(shaderProgram);

    ::glUseProgram(shaderProgram);

    ::glDeleteShader(vertexShader);
    ::glDeleteShader(fragmentShader);

    m_cameraRightLocation = ::glGetUniformLocation(shaderProgram, "cameraRight");
    m_cameraUpLocation = ::glGetUniformLocation(shaderProgram, "cameraUp");
    m_viewProjectionLocation = ::glGetUniformLocation(shaderProgram, "viewProjection");
}

void Application::InitOpenGlBuffers() {
    static constexpr GLfloat VertexBufferData[] = {
        -0.5f, -0.5f, 0.0f,
        +0.5f, -0.5f, 0.0f,
        -0.5f, +0.5f, 0.0f,
        +0.5f, +0.5f, 0.0f,
    };

    unsigned int VAO;
    ::glGenVertexArrays(1, &VAO);
    ::glBindVertexArray(VAO);

    GLuint billboardVertexBuffer;
    ::glGenBuffers(1, &billboardVertexBuffer);
    ::glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
    ::glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferData), VertexBufferData, GL_STATIC_DRAW);
    ::glVertexAttribPointer(
        0, 3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
    ::glEnableVertexAttribArray(0);

    GLuint colorParticlesBuffer;
    ::glGenBuffers(1, &colorParticlesBuffer);
    ::glBindBuffer(GL_ARRAY_BUFFER, colorParticlesBuffer);

    {
        assert(m_particles.GetBufferSize() <= UINT32_MAX);
        const auto bufferSize = static_cast<uint32_t>(m_particles.GetBufferSize());

        ::glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STREAM_DRAW);
    }

    ::glVertexAttribPointer(
        1, 3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ColorParticle),
        reinterpret_cast<void*>(offsetof(ColorParticle, pos))
    );
    ::glVertexAttribPointer(
        2, 1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ColorParticle),
        reinterpret_cast<void*>(offsetof(ColorParticle, size))
    );
    ::glVertexAttribPointer(
        3, 4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(ColorParticle),
        reinterpret_cast<void*>(offsetof(ColorParticle, color))
    );
    ::glEnableVertexAttribArray(1);
    ::glEnableVertexAttribArray(2);
    ::glEnableVertexAttribArray(3);

    ::glVertexAttribDivisor(0, 0);
    ::glVertexAttribDivisor(1, 1);
    ::glVertexAttribDivisor(2, 1);
    ::glVertexAttribDivisor(3, 1);

    ::glBindVertexArray(VAO);
}


ColorParticle Application::CreateParticle() {
    return {
        .pos = {
            2.0f * m_dist(m_mt) - 1.0f,
            2.0f * m_dist(m_mt) - 1.0f,
            2.0f * m_dist(m_mt) - 1.0f
        },
        .size = 0.04f * m_dist(m_mt),
        .color = {
            static_cast<uint8_t>(std::round(80.0f + 100.0f * std::sqrt(m_dist(m_mt)))),
            static_cast<uint8_t>(std::round(40.0f + 100.0f * std::sqrt(m_dist(m_mt)))),
            200,
            255
        }
    };
}

ParticleData Application::CreateParticleData(float lifeOffset) {
    return {
        .speed = { m_dist(m_mt), m_dist(m_mt), m_dist(m_mt) },
        .life = lifeOffset + m_dist(m_mt)
    };
}

void Application::SetCameraLocation(float distance, float hAngle, float vAngle) {
    const glm::vec3 pos = {
        distance * std::cos(hAngle) * std::sin(vAngle),
        distance * std::cos(vAngle),
        distance * std::sin(hAngle) * std::sin(vAngle)
    };
    m_camera.SetPosition(pos);
}


void Application::BeforeRender(float deltaTime) {
    UpdatePosition(deltaTime);

    if (m_cameraUpdated)
        UploadPosition();
}


void Application::Render() const {
    assert(m_particles.GetCount() <= INT32_MAX);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ::glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<int>(m_particles.GetCount()));
}


void Application::AfterRender(float deltaTime) {
    UpdateParticles(deltaTime);

    if (m_particlesUpdated)
        UploadParticles();
}

void Application::UpdateParticles(float deltaTime) {
    if (::glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        return;

    const size_t count = m_particles.GetCount();
    for (size_t i = 0; i < count; i++) {
        auto& particle = m_particles.GetParticle(i);
        auto& offlineData = m_particles.GetOfflineData(i);

        const float lifetime = offlineData.life;
        assert(lifetime > 0.0f);

        particle.pos -= particle.pos * offlineData.speed * deltaTime;
        particle.size -= particle.size * deltaTime;

        const float newLifetime = lifetime - deltaTime;

        if (newLifetime <= 0.0f) {
            particle = CreateParticle();
            offlineData = CreateParticleData(1.0f);
        } else {
            offlineData.life = newLifetime;
        }
    }

    m_particlesUpdated = true;
}

void Application::UpdatePosition(float deltaTime) {
    float hAngleSpeed = 0.0f;
    float vAngleSpeed = 0.0f;
    float distanceSpeed = 0.0f;

    if (::glfwGetKey(m_window, GLFW_KEY_A))
        hAngleSpeed += 1.0f;
    if (::glfwGetKey(m_window, GLFW_KEY_D))
        hAngleSpeed -= 1.0f;
    if (::glfwGetKey(m_window, GLFW_KEY_W))
        distanceSpeed -= 1.0f;
    if (::glfwGetKey(m_window, GLFW_KEY_S))
        distanceSpeed += 1.0f;
    if (::glfwGetKey(m_window, GLFW_KEY_Q))
        vAngleSpeed += 1.0f;
    if (::glfwGetKey(m_window, GLFW_KEY_E))
        vAngleSpeed -= 1.0f;


    if (hAngleSpeed == 0.0f && vAngleSpeed == 0.0f && distanceSpeed == 0.0f)
        return;

    m_cameraDistance = std::clamp(m_cameraDistance + distanceSpeed * deltaTime, 2.0f, 6.0f);
    m_cameraHAngle = std::fmod(m_cameraHAngle + hAngleSpeed * deltaTime, TWO_PI);
    m_cameraVAngle = std::clamp(m_cameraVAngle + vAngleSpeed * deltaTime, 0.0f + 0.75f, PI - 0.75f);

    SetCameraLocation(m_cameraDistance, m_cameraHAngle, m_cameraVAngle);

    m_cameraUpdated = true;
}

void Application::UploadParticles() {
    assert(m_particles.GetBufferSize() <= UINT32_MAX);

    const auto bufferSize = static_cast<uint32_t>(m_particles.GetBufferSize());
    ::glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, m_particles.GetParticlesBuffer());

    m_particlesUpdated = false;
}

void Application::UploadPosition() {
    const auto view = m_camera.View();
    const auto projection = m_camera.Projection();

    ::glUniform3f(m_cameraRightLocation, view[0][0], view[1][0], view[2][0]);
    ::glUniform3f(m_cameraUpLocation, view[0][1], view[1][1], view[2][1]);
    ::glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection * view));

    m_cameraUpdated = false;
}

Application::Application(const ApplicationConfiguration& config) : m_particles(0) {
    InitWindow(config.width, config.height, config.name);
    InitParticles(config.particleCount);
    InitShaders();
    InitOpenGlBuffers();

    SetCameraLocation(m_cameraDistance, m_cameraHAngle, m_cameraVAngle);
}

Application::~Application() noexcept {
    if (m_window) ::glfwDestroyWindow(m_window);
    ::glfwTerminate();
}


int Application::Run() {
    double lastTime = glfwGetTime();
    while (!::glfwWindowShouldClose(m_window)) {
        const double newTime = glfwGetTime();
        const auto deltaTime = static_cast<float>(newTime - lastTime);

        Render();

        ::glFlush();

        AfterRender(deltaTime);

        ::glfwPollEvents();
        ::glfwSwapBuffers(m_window);

        BeforeRender(deltaTime);

        lastTime = newTime;
    }

    return 0;
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));

    ::glViewport(0, 0, width, height);
    application->m_camera.SetAspect(static_cast<float>(width) / static_cast<float>(height));
    application->m_cameraUpdated = true;
}
