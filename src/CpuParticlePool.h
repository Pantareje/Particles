#pragma once

#include <cassert>
#include <vector>

template<typename T, typename U>
class CpuParticlePool {
    size_t m_maxParticles;
    size_t m_particleCount;

    std::vector<T> m_particles;
    std::vector<U> m_offlineData;
public:
    explicit CpuParticlePool(size_t maxParticles)
        : m_maxParticles(maxParticles), m_particleCount(0),
          m_particles(maxParticles), m_offlineData(maxParticles) {}

    [[nodiscard]] size_t GetBufferSize() noexcept { return m_particles.size() * sizeof(T); }

    [[nodiscard]] size_t GetMaxCount() noexcept { return m_maxParticles; }
    [[nodiscard]] size_t GetCount() const noexcept { return m_particleCount; }


    [[nodiscard]] T& GetParticle(size_t i) noexcept { return m_particles[i]; }
    [[nodiscard]] const T& GetParticle(size_t i) const noexcept { return m_particles[i]; }

    [[nodiscard]] U& GetOfflineData(size_t i) noexcept { return m_offlineData[i]; }
    [[nodiscard]] const U& GetOfflineData(size_t i) const noexcept { return m_offlineData[i]; }


    [[nodiscard]] const T* GetParticlesBuffer() const noexcept { return m_particles.data(); }


    void AddParticle(const T& particle, const U& offlineData) {
        if (m_particleCount == m_maxParticles) return;

        m_particles[m_particleCount] = particle;
        m_offlineData[m_particleCount] = offlineData;

        m_particleCount += 1;
    }
};
