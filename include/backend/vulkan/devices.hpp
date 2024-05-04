#ifndef _HSJE_VULKAN_DEVICES_HPP__
#define _HSJE_VULKAN_DEVICES_HPP__

#include "common.hpp"
#include "instance.hpp"
#include "queue_family.hpp"

namespace hsje {
  namespace vk {
    class PhysicalDevice {
      public:
        PhysicalDevice(const std::shared_ptr<Instance>& instance);
        void choose();
        VkPhysicalDevice get() const;

      public:
        static int rate_suitability(const VkPhysicalDevice& device);
        static bool is_suitable(const VkPhysicalDevice& device);

      private:
        std::shared_ptr<Instance> instance;
        VkPhysicalDevice device;
    };

    class LogicalDevice {
      public:
        LogicalDevice(const std::shared_ptr<PhysicalDevice>& device);
        virtual ~LogicalDevice();
        VkDevice get() const;
      private:
        VkDevice device;
    };

    class Devices {
      public:
        Devices(const std::shared_ptr<Instance>& instance);
        virtual ~Devices() = default;

      private:
        std::shared_ptr<PhysicalDevice> physical;
        std::shared_ptr<LogicalDevice>  logical;
        std::shared_ptr<Instance>       instance;
        QueueFamily                     queue_family;
    };
  }
}

#endif // _HSJE_VULKAN_DEVICES_HPP__
