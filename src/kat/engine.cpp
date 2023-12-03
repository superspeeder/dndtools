#include "engine.hpp"

#include <unordered_set>
#include <spdlog/spdlog.h>

#include "window.hpp"

namespace kat {
    bool queue_families::is_complete() const {
        return graphics.has_value() && present.has_value() && transfer.has_value() && compute.has_value();
    }

    engine::engine() {
        glfwInit();

        create_instance();
        select_physical_device();
        select_queue_families();
        create_device();
    }

    engine::~engine() {
        safe_destroy(m_device);
        safe_destroy(m_instance);
    }

    std::shared_ptr<engine> engine::create() {
        return std::shared_ptr<engine>(new engine());
    }

    void engine::create_instance() {
        constexpr vk::ApplicationInfo application_info = vk::ApplicationInfo().setApiVersion(VK_API_VERSION_1_3);

        std::vector<const char *> required_extensions = get_required_instance_extensions();

        vk::InstanceCreateInfo instance_create_info{};
        instance_create_info.setPApplicationInfo(&application_info);
        instance_create_info.setPEnabledExtensionNames(required_extensions);

        m_instance = vk::createInstance(instance_create_info);
    }

    void engine::select_physical_device() {
        m_physical_device = m_instance.enumeratePhysicalDevices().front();
        spdlog::debug("GPU: {}", m_physical_device.getProperties().deviceName.data());
    }

    void engine::select_queue_families() {
        const auto queue_family_properties = m_physical_device.getQueueFamilyProperties();
        uint32_t   index                   = 0;

        for (const auto &qfp : queue_family_properties) {
            if (qfp.queueFlags & vk::QueueFlagBits::eGraphics) {
                m_queue_families.graphics = index;
            }

            if (qfp.queueFlags & vk::QueueFlagBits::eTransfer) {
                m_queue_families.transfer = index;
            }

            if (qfp.queueFlags & vk::QueueFlagBits::eCompute) {
                m_queue_families.compute = index;
            }

            if (get_physical_device_presentation_support(m_instance, m_physical_device, index)) {
                m_queue_families.present = index;
            }

            if (m_queue_families.is_complete()) {
                break;
            }
        }

        if (!m_queue_families.is_complete()) {
            throw std::runtime_error("Failed to find queue families");
        }

        spdlog::debug("Selected queue families: (graphics: {}, present: {}, transfer: {}, compute: {}).",
                      *m_queue_families.graphics, *m_queue_families.present, *m_queue_families.transfer,
                      *m_queue_families.compute);
    }

    void engine::create_device() {
        const std::unordered_set<uint32_t> families = { *m_queue_families.graphics, *m_queue_families.present,
                                                        *m_queue_families.transfer, *m_queue_families.compute };

        float queue_priority = 1.0f;

        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

        for (const auto &family : families) {
            queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags{}, family, 1, &queue_priority);
        }

        std::vector<const char *> extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        vk::PhysicalDeviceFeatures2 features{};
        features.features.largePoints        = true;
        features.features.wideLines          = true;
        features.features.geometryShader     = true;
        features.features.tessellationShader = true;

        vk::PhysicalDeviceVulkan13Features vk_13_features{};
        vk_13_features.dynamicRendering = true;
        features.pNext                  = &vk_13_features;

        m_device = m_physical_device.createDevice(vk::DeviceCreateInfo{ vk::DeviceCreateFlags{}, queue_create_infos, {},
                                                                        extensions, nullptr,
                                                                        &features });

        m_queues.graphics = m_device.getQueue(*m_queue_families.graphics, 0);
        m_queues.present  = m_device.getQueue(*m_queue_families.present, 0);
        m_queues.transfer = m_device.getQueue(*m_queue_families.transfer, 0);
        m_queues.compute  = m_device.getQueue(*m_queue_families.compute, 0);

        spdlog::info("Connected to graphics device");
    }
} // kat
