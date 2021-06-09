#include <string>
#include <vector>
#include "magic_enum.hpp"

#include "VulkanManager.h"
#include "Log.h"


namespace Penjin {

	void AssertVulkan(VkResult result, std::string errorText)
	{
		if (result != VkResult::VK_SUCCESS)
			throw std::exception((errorText + " [Error code: " + std::to_string(result) + " (" + (std::string)magic_enum::enum_name(result) + ")]").c_str());
	}

	VkInstance VulkanManager::instance{};
	VkDevice VulkanManager::device{};
	uint32_t VulkanManager::queueFamilyIndex = -1;
	uint32_t VulkanManager::queueCount = 0;

	void VulkanManager::Init(std::string name, uint32_t version)
	{

		uint32_t amountLayers;
		vkEnumerateInstanceLayerProperties(&amountLayers, nullptr);
		VkLayerProperties* layerProperties = new VkLayerProperties[amountLayers];
		vkEnumerateInstanceLayerProperties(&amountLayers, layerProperties);

		Log::Message("_______________________====================__________________________________");
		Log::Message("                       || INSANCE LAYERS ||");
		Log::Message("-----------------------====================----------------------------------");
		for (size_t i = 0; i < amountLayers; i++)
		{
			Log::Message("Layer ID               | " + std::to_string(i));
			PrintLayerInfo(layerProperties[i]);
		}
		Log::Message("");
		Log::Message("");
		delete[] layerProperties;

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

	void VulkanManager::FindQueueFamily(VkPhysicalDevice device)
	{
		uint32_t amountQueueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFamilies, nullptr);
		VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[amountQueueFamilies];
		vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFamilies, queueFamilyProperties);

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

		if (VulkanManager::queueFamilyIndex < 0)
			throw std::exception("No physical device meets the requirements!");
		if (VulkanManager::queueCount <= 0)
			throw std::exception(("The selected QueueFamily[" + std::to_string(VulkanManager::queueFamilyIndex) + "] has no queues").c_str());

		Log::Message("Selected QueueFamily " + std::to_string(VulkanManager::queueFamilyIndex) + " with " + std::to_string(VulkanManager::queueCount) + " queues.");
	}
	void VulkanManager::CreateDevice()
	{
		uint32_t amountPhysicalDevices = GetPhysicalDeviceCount();
		VkPhysicalDevice* physicalDevices = GetPhysicalDevices(amountPhysicalDevices);

		Log::Message("===[ PHYSICAL DEVICES ]===");
		for (size_t i = 0; i < amountPhysicalDevices; i++)
			PrintDeviceInfo(physicalDevices[i]);


		VkPhysicalDevice selectedPhysicalDevice = physicalDevices[0]; //ToDo: pick best device instead of first device

		FindQueueFamily(selectedPhysicalDevice);

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
		VkResult result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &VulkanManager::device);
		delete[] physicalDevices;
		AssertVulkan(result, "Unable to create device");

	}

	void VulkanManager::PrintLayerInfo(VkLayerProperties layerProperties)
	{
		Log::Message("Name                   | " + std::string(layerProperties.layerName));
		Log::Message("SpecVersion            | " + std::to_string(layerProperties.specVersion));
		Log::Message("ImplementationVersion  | " + std::to_string(layerProperties.implementationVersion));
		Log::Message("Description            | " + std::string(layerProperties.description));

		uint32_t amountExtensions;
		vkEnumerateInstanceExtensionProperties(layerProperties.layerName, &amountExtensions, nullptr);
		if (amountExtensions > 0)
		{
			VkExtensionProperties* extensionProperties = new VkExtensionProperties[amountExtensions];
			vkEnumerateInstanceExtensionProperties(layerProperties.layerName, &amountExtensions, extensionProperties);

			Log::Message("  _____________________|_________");
			Log::Message(" |         Extensions");
			for (size_t i = 0; i < amountExtensions; i++)
			{
				Log::Message(" |---------|---------------------");
				Log::Message(" | Name    | " + std::string(extensionProperties[i].extensionName));
				Log::Message(" | Version | " + std::to_string(extensionProperties[i].specVersion));
			}
			Log::Message(" |_________|_____________________");
			delete[] extensionProperties;
		}
		Log::Message("_______________________|_____________________________________________________");
	}

	void VulkanManager::PrintDeviceInfo(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		Log::Message("Name: " + std::string(properties.deviceName));
		//uint32_t apiVer = properties.apiVersion;
		//std::cout << "Api version: " << VK_VERSION_MAJOR(apiVer) << "." << VK_VERSION_MINOR(apiVer) << "." << VK_VERSION_PATCH(apiVer) << std::endl;
		//std::cout << "device type: " << properties.deviceType << std::endl;

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);
		Log::Message("");
		Log::Message("===[ FEATURES ]===");

		Log::Message("[" + std::string((features.robustBufferAccess == 1 ? "+" : "-")) + "] " + "robustBufferAccess" );
		Log::Message("[" + std::string((features.fullDrawIndexUint32 == 1 ? "+" : "-")) + "] " + "fullDrawIndexUint32" );
		Log::Message("[" + std::string((features.imageCubeArray == 1 ? "+" : "-")) + "] " + "imageCubeArray" );
		Log::Message("[" + std::string((features.independentBlend == 1 ? "+" : "-")) + "] " + "independentBlend" );
		Log::Message("[" + std::string((features.geometryShader == 1 ? "+" : "-")) + "] " + "geometryShader" );
		Log::Message("[" + std::string((features.tessellationShader == 1 ? "+" : "-")) + "] " + "tessellationShader" );
		Log::Message("[" + std::string((features.sampleRateShading == 1 ? "+" : "-")) + "] " + "sampleRateShading" );
		Log::Message("[" + std::string((features.dualSrcBlend == 1 ? "+" : "-")) + "] " + "dualSrcBlend" );
		Log::Message("[" + std::string((features.logicOp == 1 ? "+" : "-")) + "] " + "logicOp" );
		Log::Message("[" + std::string((features.multiDrawIndirect == 1 ? "+" : "-")) + "] " + "multiDrawIndirect" );
		Log::Message("[" + std::string((features.drawIndirectFirstInstance == 1 ? "+" : "-")) + "] " + "drawIndirectFirstInstance" );
		Log::Message("[" + std::string((features.depthClamp == 1 ? "+" : "-")) + "] " + "depthClamp" );
		Log::Message("[" + std::string((features.depthBiasClamp == 1 ? "+" : "-")) + "] " + "depthBiasClamp" );
		Log::Message("[" + std::string((features.fillModeNonSolid == 1 ? "+" : "-")) + "] " + "fillModeNonSolid" );
		Log::Message("[" + std::string((features.depthBounds == 1 ? "+" : "-")) + "] " + "depthBounds" );
		Log::Message("[" + std::string((features.wideLines == 1 ? "+" : "-")) + "] " + "wideLines" );
		Log::Message("[" + std::string((features.largePoints == 1 ? "+" : "-")) + "] " + "largePoints" );
		Log::Message("[" + std::string((features.alphaToOne == 1 ? "+" : "-")) + "] " + "alphaToOne" );
		Log::Message("[" + std::string((features.multiViewport == 1 ? "+" : "-")) + "] " + "multiViewport" );
		Log::Message("[" + std::string((features.samplerAnisotropy == 1 ? "+" : "-")) + "] " + "samplerAnisotropy" );
		Log::Message("[" + std::string((features.textureCompressionETC2 == 1 ? "+" : "-")) + "] " + "textureCompressionETC2" );
		Log::Message("[" + std::string((features.textureCompressionASTC_LDR == 1 ? "+" : "-")) + "] " + "textureCompressionASTC_LDR" );
		Log::Message("[" + std::string((features.textureCompressionBC == 1 ? "+" : "-")) + "] " + "textureCompressionBC" );
		Log::Message("[" + std::string((features.occlusionQueryPrecise == 1 ? "+" : "-")) + "] " + "occlusionQueryPrecise" );
		Log::Message("[" + std::string((features.pipelineStatisticsQuery == 1 ? "+" : "-")) + "] " + "pipelineStatisticsQuery" );
		Log::Message("[" + std::string((features.vertexPipelineStoresAndAtomics == 1 ? "+" : "-")) + "] " + "vertexPipelineStoresAndAtomics" );
		Log::Message("[" + std::string((features.fragmentStoresAndAtomics == 1 ? "+" : "-")) + "] " + "fragmentStoresAndAtomics" );
		Log::Message("[" + std::string((features.shaderTessellationAndGeometryPointSize == 1 ? "+" : "-")) + "] " + "shaderTessellationAndGeometryPointSize" );
		Log::Message("[" + std::string((features.shaderImageGatherExtended == 1 ? "+" : "-")) + "] " + "shaderImageGatherExtended" );
		Log::Message("[" + std::string((features.shaderStorageImageExtendedFormats == 1 ? "+" : "-")) + "] " + "shaderStorageImageExtendedFormats" );
		Log::Message("[" + std::string((features.shaderStorageImageMultisample == 1 ? "+" : "-")) + "] " + "shaderStorageImageMultisample" );
		Log::Message("[" + std::string((features.shaderStorageImageReadWithoutFormat == 1 ? "+" : "-")) + "] " + "shaderStorageImageReadWithoutFormat" );
		Log::Message("[" + std::string((features.shaderStorageImageWriteWithoutFormat == 1 ? "+" : "-")) + "] " + "shaderStorageImageWriteWithoutFormat" );
		Log::Message("[" + std::string((features.shaderUniformBufferArrayDynamicIndexing == 1 ? "+" : "-")) + "] " + "shaderUniformBufferArrayDynamicIndexing" );
		Log::Message("[" + std::string((features.shaderSampledImageArrayDynamicIndexing == 1 ? "+" : "-")) + "] " + "shaderSampledImageArrayDynamicIndexing" );
		Log::Message("[" + std::string((features.shaderStorageBufferArrayDynamicIndexing == 1 ? "+" : "-")) + "] " + "shaderStorageBufferArrayDynamicIndexing" );
		Log::Message("[" + std::string((features.shaderStorageImageArrayDynamicIndexing == 1 ? "+" : "-")) + "] " + "shaderStorageImageArrayDynamicIndexing" );
		Log::Message("[" + std::string((features.shaderClipDistance == 1 ? "+" : "-")) + "] " + "shaderClipDistance" );
		Log::Message("[" + std::string((features.shaderCullDistance == 1 ? "+" : "-")) + "] " + "shaderCullDistance" );
		Log::Message("[" + std::string((features.shaderFloat64 == 1 ? "+" : "-")) + "] " + "shaderFloat64" );
		Log::Message("[" + std::string((features.shaderInt64 == 1 ? "+" : "-")) + "] " + "shaderInt64" );
		Log::Message("[" + std::string((features.shaderInt16 == 1 ? "+" : "-")) + "] " + "shaderInt16" );
		Log::Message("[" + std::string((features.shaderResourceResidency == 1 ? "+" : "-")) + "] " + "shaderResourceResidency" );
		Log::Message("[" + std::string((features.shaderResourceMinLod == 1 ? "+" : "-")) + "] " + "shaderResourceMinLod" );
		Log::Message("[" + std::string((features.sparseBinding == 1 ? "+" : "-")) + "] " + "sparseBinding" );
		Log::Message("[" + std::string((features.sparseResidencyBuffer == 1 ? "+" : "-")) + "] " + "sparseResidencyBuffer" );
		Log::Message("[" + std::string((features.sparseResidencyImage2D == 1 ? "+" : "-")) + "] " + "sparseResidencyImage2D" );
		Log::Message("[" + std::string((features.sparseResidencyImage3D == 1 ? "+" : "-")) + "] " + "sparseResidencyImage3D" );
		Log::Message("[" + std::string((features.sparseResidency2Samples == 1 ? "+" : "-")) + "] " + "sparseResidency2Samples" );
		Log::Message("[" + std::string((features.sparseResidency4Samples == 1 ? "+" : "-")) + "] " + "sparseResidency4Samples" );
		Log::Message("[" + std::string((features.sparseResidency8Samples == 1 ? "+" : "-")) + "] " + "sparseResidency8Samples" );
		Log::Message("[" + std::string((features.sparseResidency16Samples == 1 ? "+" : "-")) + "] " + "sparseResidency16Samples" );
		Log::Message("[" + std::string((features.sparseResidencyAliased == 1 ? "+" : "-")) + "] " + "sparseResidencyAliased" );
		Log::Message("[" + std::string((features.variableMultisampleRate == 1 ? "+" : "-")) + "] " + "variableMultisampleRate" );
		Log::Message("[" + std::string((features.inheritedQueries == 1 ? "+" : "-")) + "] " + "inheritedQueries" );

		Log::Message("");


		uint32_t amountQueueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFamilies, nullptr);
		VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[amountQueueFamilies];
		vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFamilies, queueFamilyProperties);

		for (size_t i = 0; i < amountQueueFamilies; i++)
		{
			Log::Message("===[ Queue Family " + std::to_string(i) + " ]===" );
			Log::Message("Queue count:    " + std::to_string(queueFamilyProperties[i].queueCount ));
			Log::Message("Graphics:       " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT != 0) ));
			Log::Message("Compute:        " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT != 0) ));
			Log::Message("Transfer:       " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT != 0) ));
			Log::Message("PROTECTED:      " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_PROTECTED_BIT != 0) ));
			Log::Message("SPARSE_BINDING: " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_SPARSE_BINDING_BIT != 0) ));
			Log::Message("");
		}

		Log::Message("");

		delete[] queueFamilyProperties;
	}

}