#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::vec3 m_up;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

public:
    Camera() = default;

    Camera(
        const glm::vec3& cameraPos,
        const glm::vec3& cameraTarget,
        const glm::vec3& cameraUp,
        float fov, float aspect,
        float near, float far
    ) : m_pos(cameraPos),
        m_target(cameraTarget),
        m_up(cameraUp),
        m_fov(fov), m_aspect(aspect),
        m_near(near), m_far(far) {}

    void SetPosition(const glm::vec3& pos) { m_pos = pos; }
    void SetAspect(float aspect) { m_aspect = aspect; }

    [[nodiscard]] glm::mat4 View() const { return glm::lookAt(m_pos, m_target, m_up); }
    [[nodiscard]] glm::mat4 Projection() const { return glm::perspective(m_fov, m_aspect, m_near, m_far); }
};
