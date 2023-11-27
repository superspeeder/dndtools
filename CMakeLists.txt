cmake_minimum_required(VERSION 3.27)
project(dndtools)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(eventpp CONFIG REQUIRED)

add_executable(dndtools src/main.cpp
        src/kat/window.cpp
        src/kat/window.hpp
        src/kat/core.hpp
        src/kat/math.cpp
        src/kat/math.hpp
)
target_include_directories(dndtools PRIVATE src/ ${Stb_INCLUDE_DIR})
target_link_libraries(dndtools glm::glm glfw spdlog::spdlog Vulkan::Vulkan eventpp::eventpp)
target_compile_definitions(dndtools PRIVATE -DGLFW_INCLUDE_VULKAN)