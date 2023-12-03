#pragma once
#include <optional>

#include "core.hpp"

namespace kat {

    struct queue_families {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        std::optional<uint32_t> transfer;
        std::optional<uint32_t> compute;

        [[nodiscard]] bool is_complete() const;
    };

    struct queues {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue transfer;
        vk::Queue compute;
    };

    template <typename T>
    concept instance_destructible = requires(const vk::Instance &instance, const T &object)
    {
        instance.destroy(object);
    };

    template <typename T>
    concept device_destructible = requires(const vk::Device &device, const T &object)
    {
        device.destroy(object);
    };

    template <typename T, typename E>
    concept safe_destructible = requires(const T &object, const E *e)
    {
        { e->destroy(object) };
        { object.operator bool() } -> std::same_as<bool>;
    };

    class engine;

    class engine {
        engine();

    public:
        ~engine();

        static std::shared_ptr<engine> create();

        template <instance_destructible T>
        void destroy(const T &object) const {
            m_instance.destroy(object);
        }

        template <device_destructible T>
        void destroy(const T &object) const {
            m_device.destroy(object);
        }

        inline void destroy(const vk::Instance &instance) const {
            instance.destroy();
        }

        inline void destroy(const vk::Device &device) const {
            device.destroy();
        }

        template <safe_destructible<engine> T>
        void safe_destroy(const T &object) const {
            if (object)
                destroy(object);
        }

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
