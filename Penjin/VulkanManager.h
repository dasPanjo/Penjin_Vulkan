#pragma once
#include <vulkan/vulkan.h>
#include <iostream>

namespace Penjin
{
	static class VulkanManager
	{
	public:
		static void Init(std::string name, uint32_t version);
		static void Cleanup();

		static VkInstance const GetInstance() { return instance; }
		static VkDevice const GetDevice() { return device; }
	private:
		static VkInstance instance;
		static VkDevice device;
		static VkPhysicalDevice physicalDevice;

		static uint32_t queueFamilyIndex;
		static uint32_t queueCount;

		static void CreateInstance(std::string name, uint32_t version);
		static void CreateDevice();

		static uint32_t GetPhysicalDeviceCount();
		static VkPhysicalDevice* GetPhysicalDevices(uint32_t amountPhysicalDevices);
		static void FindQueueFamily();

	};
}

