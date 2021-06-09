#pragma once
#include <vulkan/vulkan.h>

namespace Penjin {

	class VulkanLogger
	{
		public:
			static void DisplayDevices(VkPhysicalDevice* physicalDevices, uint32_t physicalDeviceCount);
			static void LogDeviceInfo(VkPhysicalDevice physicalDevice);

			static void LogLayerInfo(VkLayerProperties layerProperties);
			static void DisplayLayers();
	};
}
