#include "VulkanManager.h"

#include <string>
#include <vector>
#include "magic_enum.hpp"

#include "VulkanLogger.h"
#include "Log.h"


namespace Penjin {

	void AssertVulkan(VkResult result, std::string errorText)
	{
		if (result != VkResult::VK_SUCCESS)
			throw std::exception((errorText + " [Error code: " + std::to_string(result) + " (" + (std::string)magic_enum::enum_name(result) + ")]").c_str());
	}

	VkInstance VulkanManager::instance{};
	VkDevice VulkanManager::device{};
	VkPhysicalDevice VulkanManager::physicalDevice{};

	uint32_t VulkanManager::queueFamilyIndex = -1;
	uint32_t VulkanManager::queueCount = 0;

	void VulkanManager::Init(std::string name, uint32_t version)
	{
		VulkanLogger::DisplayLayers();
		CreateInstance(name, version);
		CreateDevice();

		VkQueue queue = {};
		vkGetDeviceQueue(VulkanManager::device, VulkanManager::queueFamilyIndex, 0, &queue);

	}

	void VulkanManager::Cleanup()
	{
		vkDeviceWaitIdle(VulkanManager::device);
		vkDestroyDevice(VulkanManager::device, nullptr);
		vkDestroyInstance(VulkanManager::instance, nullptr);
	}

	void VulkanManager::CreateInstance(std::string name, uint32_t version)
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = name.c_str();
		appInfo.applicationVersion = version;
		appInfo.pEngineName = "Penjin";
		appInfo.apiVersion = VK_API_VERSION_1_2;

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation",
		};

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.enabledExtensionCount = 0;
		createInfo.ppEnabledExtensionNames = nullptr;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &VulkanManager::instance);
		AssertVulkan(result, "Unable to create vkIntance");

	}

	uint32_t VulkanManager::GetPhysicalDeviceCount()
	{
		uint32_t physicalDeviceCount = 0;
		VkResult result = vkEnumeratePhysicalDevices(VulkanManager::instance, &physicalDeviceCount, nullptr);
		AssertVulkan(result, "Unable to fetch amount of physical devices.");
		if (physicalDeviceCount <= 0)
			throw std::exception(("VulkanManager::GetPhysicalDeviceCount() returned " + std::to_string(physicalDeviceCount)).c_str());
		return physicalDeviceCount;
	}

	VkPhysicalDevice* VulkanManager::GetPhysicalDevices(uint32_t amountPhysicalDevices)
	{
		VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[amountPhysicalDevices];
		VkResult result = vkEnumeratePhysicalDevices(VulkanManager::instance, &amountPhysicalDevices, physicalDevices);
		AssertVulkan(result, "Unable to fetch " + std::to_string(amountPhysicalDevices) + " physical devices");
		return physicalDevices;
	}

	void VulkanManager::FindQueueFamily()
	{
		uint32_t physicalDevicesCount = GetPhysicalDeviceCount();
		VkPhysicalDevice* physicalDevices = GetPhysicalDevices(physicalDevicesCount);

		VulkanLogger::DisplayDevices(physicalDevices, physicalDevicesCount);

		VulkanManager::physicalDevice = physicalDevices[0]; //ToDo: pick best device instead of first device

		uint32_t amountQueueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(VulkanManager::physicalDevice, &amountQueueFamilies, nullptr);
		VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[amountQueueFamilies];
		vkGetPhysicalDeviceQueueFamilyProperties(VulkanManager::physicalDevice, &amountQueueFamilies, queueFamilyProperties);

		VulkanManager::queueFamilyIndex = -1;
		VulkanManager::queueCount = 0;

		for (size_t i = 0; i < amountQueueFamilies; i++)
		{

			const VkQueueFamilyProperties& family = queueFamilyProperties[i];
			if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				VulkanManager::queueFamilyIndex = i;   //Found the family, record its index
				VulkanManager::queueCount = family.queueCount > 1 ? 1 : family.queueCount;
				break;
			}
			i++;
		}
		delete[] queueFamilyProperties;
		delete[] physicalDevices;

		if (VulkanManager::queueFamilyIndex < 0)
			throw std::exception("No physical device meets the requirements!");
		if (VulkanManager::queueCount <= 0)
			throw std::exception(("The selected QueueFamily[" + std::to_string(VulkanManager::queueFamilyIndex) + "] has no queues").c_str());

		Log::Message("Selected QueueFamily " + std::to_string(VulkanManager::queueFamilyIndex) + " with " + std::to_string(VulkanManager::queueCount) + " queues.");
	}
	void VulkanManager::CreateDevice()
	{
		FindQueueFamily();

		float* queuePriority = new float[VulkanManager::queueCount];
		for (size_t i = 0; i < VulkanManager::queueCount; i++)
		{
			queuePriority[i] = 1.0f;
		}

		VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.pNext = nullptr;
		deviceQueueCreateInfo.flags = 0;
		deviceQueueCreateInfo.queueFamilyIndex = VulkanManager::queueFamilyIndex;
		deviceQueueCreateInfo.queueCount = VulkanManager::queueCount;
		deviceQueueCreateInfo.pQueuePriorities = queuePriority;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = nullptr;
		deviceCreateInfo.flags = 0;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;
		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = nullptr;

		VkPhysicalDeviceFeatures deviceFeatures = { VK_FALSE };
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		VkResult result = vkCreateDevice(VulkanManager::physicalDevice, &deviceCreateInfo, nullptr, &VulkanManager::device);
		AssertVulkan(result, "Unable to create device");

	}


}