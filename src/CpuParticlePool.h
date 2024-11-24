#pragma once

#include <cassert>
#include <vector>

/*
 * En este ejemplo, las partículas no son destruidas sino reiniciadas a un nuevo estado
 * inicial. Si la simulación de partículas requiriera crear y destruir partículas de
 * manera arbitraria durante la simulación se debería usar otra estructura, como por
 * ejemplo una lista libre, la cual permite crear y destruir objetos en un vector de
 * manera arbitraria con tiempo de ejecución O(1).
 */

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
    [[nodiscard]] U& GetOfflineData(size_t i) noexcept { return m_offlineData[i]; }
    [[nodiscard]] const T* GetParticlesBuffer() const noexcept { return m_particles.data(); }

    void AddParticle(const T& particle, const U& offlineData) {
        if (m_particleCount == m_maxParticles) return;

        m_particles[m_particleCount] = particle;
        m_offlineData[m_particleCount] = offlineData;

        m_particleCount += 1;
    }
};
