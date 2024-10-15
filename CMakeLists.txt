cmake_minimum_required(VERSION 3.11)
project(Particles)

include(cmake/EmbedResources.cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(external/glad)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Particles WIN32
        src/Main.cpp
        src/ParticleShaders.h
        src/Camera.h
        src/CpuParticlePool.h
        src/Application.cpp
        src/Application.h
        src/Particle.h
        src/Math.h
        src/ApplicationConfiguration.h)

target_link_libraries(Particles PRIVATE glad glfw)
