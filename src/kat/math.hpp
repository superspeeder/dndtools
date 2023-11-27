#pragma once

#include <glm/glm.hpp>

namespace kat {

    template <typename T>
    concept numeric = std::integral<T> || std::floating_point<T>;

    template <numeric T>
    struct rect {
        glm::tvec2<T> position;
        glm::tvec2<T> size;

        // TODO: math
    };
}
