#pragma once
#include <optional>

#include "core.hpp"

namespace kat {

    struct queue_families {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        std::optional<uint32_t> transfer;
        std::optional<uint32_t> compute;

        bool is_complete() const;
    };

    struct queues {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue transfer;
        vk::Queue compute;
    };

    class engine {
    public:
        engine();
        ~engine();

    private:
        vk::Instance       m_instance;
        vk::PhysicalDevice m_physical_device;
        vk::Device         m_device;
        queue_families     m_queue_families{};
        queues             m_queues{};

        void create_instance();
        void select_physical_device();
        void select_queue_families();
        void create_device();
    };

} // kat
