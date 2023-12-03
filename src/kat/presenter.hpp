#pragma once
#include "engine.hpp"
#include "window.hpp"

namespace kat {

    class presenter {
    public:
        presenter(const std::shared_ptr<engine> &engine, const std::shared_ptr<window> &window);
        ~presenter();

    private:

    };

} // kat
