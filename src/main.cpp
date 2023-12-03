#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <memory>

#include "kat/engine.hpp"
#include "kat/window.hpp"

int main() {
    auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/main.log", UINT64_MAX, 30, true);
    const std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(
        "dndtools", spdlog::sinks_init_list{ color_sink, file_sink });

    logger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(logger);

    spdlog::info("Hello!");
    {
        std::shared_ptr<kat::engine> engine = kat::engine::create();

        {
            const auto window = kat::window::create(kat::window_settings{ .title = "D&D Tools" });

            while (window->is_open()) {
                kat::poll_events();
            }
        }
    }
    spdlog::shutdown();

    return 0;
}
