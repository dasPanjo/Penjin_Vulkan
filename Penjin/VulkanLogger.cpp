#include "VulkanLogger.h"
#include "Log.h"

namespace Penjin {

	void VulkanLogger::LogLayerInfo(VkLayerProperties layerProperties)
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

	void VulkanLogger::DisplayDevices(VkPhysicalDevice* physicalDevices, uint32_t physicalDeviceCount)
	{
		Log::Message("===[ PHYSICAL DEVICES ]===");
		for (size_t i = 0; i < physicalDeviceCount; i++)
			VulkanLogger::LogDeviceInfo(physicalDevices[i]);
	}

	void VulkanLogger::LogDeviceInfo(VkPhysicalDevice device)
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

		Log::Message("[" + std::string((features.robustBufferAccess == VK_TRUE ? "+" : "-")) + "] " + "robustBufferAccess");
		Log::Message("[" + std::string((features.fullDrawIndexUint32 == VK_TRUE ? "+" : "-")) + "] " + "fullDrawIndexUint32");
		Log::Message("[" + std::string((features.imageCubeArray == VK_TRUE ? "+" : "-")) + "] " + "imageCubeArray");
		Log::Message("[" + std::string((features.independentBlend == VK_TRUE ? "+" : "-")) + "] " + "independentBlend");
		Log::Message("[" + std::string((features.geometryShader == VK_TRUE ? "+" : "-")) + "] " + "geometryShader");
		Log::Message("[" + std::string((features.tessellationShader == VK_TRUE ? "+" : "-")) + "] " + "tessellationShader");
		Log::Message("[" + std::string((features.sampleRateShading == VK_TRUE ? "+" : "-")) + "] " + "sampleRateShading");
		Log::Message("[" + std::string((features.dualSrcBlend == VK_TRUE ? "+" : "-")) + "] " + "dualSrcBlend");
		Log::Message("[" + std::string((features.logicOp == VK_TRUE ? "+" : "-")) + "] " + "logicOp");
		Log::Message("[" + std::string((features.multiDrawIndirect == VK_TRUE ? "+" : "-")) + "] " + "multiDrawIndirect");
		Log::Message("[" + std::string((features.drawIndirectFirstInstance == VK_TRUE ? "+" : "-")) + "] " + "drawIndirectFirstInstance");
		Log::Message("[" + std::string((features.depthClamp == VK_TRUE ? "+" : "-")) + "] " + "depthClamp");
		Log::Message("[" + std::string((features.depthBiasClamp == VK_TRUE ? "+" : "-")) + "] " + "depthBiasClamp");
		Log::Message("[" + std::string((features.fillModeNonSolid == VK_TRUE ? "+" : "-")) + "] " + "fillModeNonSolid");
		Log::Message("[" + std::string((features.depthBounds == VK_TRUE ? "+" : "-")) + "] " + "depthBounds");
		Log::Message("[" + std::string((features.wideLines == VK_TRUE ? "+" : "-")) + "] " + "wideLines");
		Log::Message("[" + std::string((features.largePoints == VK_TRUE ? "+" : "-")) + "] " + "largePoints");
		Log::Message("[" + std::string((features.alphaToOne == VK_TRUE ? "+" : "-")) + "] " + "alphaToOne");
		Log::Message("[" + std::string((features.multiViewport == VK_TRUE ? "+" : "-")) + "] " + "multiViewport");
		Log::Message("[" + std::string((features.samplerAnisotropy == VK_TRUE ? "+" : "-")) + "] " + "samplerAnisotropy");
		Log::Message("[" + std::string((features.textureCompressionETC2 == VK_TRUE ? "+" : "-")) + "] " + "textureCompressionETC2");
		Log::Message("[" + std::string((features.textureCompressionASTC_LDR == VK_TRUE ? "+" : "-")) + "] " + "textureCompressionASTC_LDR");
		Log::Message("[" + std::string((features.textureCompressionBC == VK_TRUE ? "+" : "-")) + "] " + "textureCompressionBC");
		Log::Message("[" + std::string((features.occlusionQueryPrecise == VK_TRUE ? "+" : "-")) + "] " + "occlusionQueryPrecise");
		Log::Message("[" + std::string((features.pipelineStatisticsQuery == VK_TRUE ? "+" : "-")) + "] " + "pipelineStatisticsQuery");
		Log::Message("[" + std::string((features.vertexPipelineStoresAndAtomics == VK_TRUE ? "+" : "-")) + "] " + "vertexPipelineStoresAndAtomics");
		Log::Message("[" + std::string((features.fragmentStoresAndAtomics == VK_TRUE ? "+" : "-")) + "] " + "fragmentStoresAndAtomics");
		Log::Message("[" + std::string((features.shaderTessellationAndGeometryPointSize == VK_TRUE ? "+" : "-")) + "] " + "shaderTessellationAndGeometryPointSize");
		Log::Message("[" + std::string((features.shaderImageGatherExtended == VK_TRUE ? "+" : "-")) + "] " + "shaderImageGatherExtended");
		Log::Message("[" + std::string((features.shaderStorageImageExtendedFormats == VK_TRUE ? "+" : "-")) + "] " + "shaderStorageImageExtendedFormats");
		Log::Message("[" + std::string((features.shaderStorageImageMultisample == VK_TRUE ? "+" : "-")) + "] " + "shaderStorageImageMultisample");
		Log::Message("[" + std::string((features.shaderStorageImageReadWithoutFormat == VK_TRUE ? "+" : "-")) + "] " + "shaderStorageImageReadWithoutFormat");
		Log::Message("[" + std::string((features.shaderStorageImageWriteWithoutFormat == VK_TRUE ? "+" : "-")) + "] " + "shaderStorageImageWriteWithoutFormat");
		Log::Message("[" + std::string((features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE ? "+" : "-")) + "] " + "shaderUniformBufferArrayDynamicIndexing");
		Log::Message("[" + std::string((features.shaderSampledImageArrayDynamicIndexing == VK_TRUE ? "+" : "-")) + "] " + "shaderSampledImageArrayDynamicIndexing");
		Log::Message("[" + std::string((features.shaderStorageBufferArrayDynamicIndexing == VK_TRUE ? "+" : "-")) + "] " + "shaderStorageBufferArrayDynamicIndexing");
		Log::Message("[" + std::string((features.shaderStorageImageArrayDynamicIndexing == VK_TRUE ? "+" : "-")) + "] " + "shaderStorageImageArrayDynamicIndexing");
		Log::Message("[" + std::string((features.shaderClipDistance == VK_TRUE ? "+" : "-")) + "] " + "shaderClipDistance");
		Log::Message("[" + std::string((features.shaderCullDistance == VK_TRUE ? "+" : "-")) + "] " + "shaderCullDistance");
		Log::Message("[" + std::string((features.shaderFloat64 == VK_TRUE ? "+" : "-")) + "] " + "shaderFloat64");
		Log::Message("[" + std::string((features.shaderInt64 == VK_TRUE ? "+" : "-")) + "] " + "shaderInt64");
		Log::Message("[" + std::string((features.shaderInt16 == VK_TRUE ? "+" : "-")) + "] " + "shaderInt16");
		Log::Message("[" + std::string((features.shaderResourceResidency == VK_TRUE ? "+" : "-")) + "] " + "shaderResourceResidency");
		Log::Message("[" + std::string((features.shaderResourceMinLod == VK_TRUE ? "+" : "-")) + "] " + "shaderResourceMinLod");
		Log::Message("[" + std::string((features.sparseBinding == VK_TRUE ? "+" : "-")) + "] " + "sparseBinding");
		Log::Message("[" + std::string((features.sparseResidencyBuffer == VK_TRUE ? "+" : "-")) + "] " + "sparseResidencyBuffer");
		Log::Message("[" + std::string((features.sparseResidencyImage2D == VK_TRUE ? "+" : "-")) + "] " + "sparseResidencyImage2D");
		Log::Message("[" + std::string((features.sparseResidencyImage3D == VK_TRUE ? "+" : "-")) + "] " + "sparseResidencyImage3D");
		Log::Message("[" + std::string((features.sparseResidency2Samples == VK_TRUE ? "+" : "-")) + "] " + "sparseResidency2Samples");
		Log::Message("[" + std::string((features.sparseResidency4Samples == VK_TRUE ? "+" : "-")) + "] " + "sparseResidency4Samples");
		Log::Message("[" + std::string((features.sparseResidency8Samples == VK_TRUE ? "+" : "-")) + "] " + "sparseResidency8Samples");
		Log::Message("[" + std::string((features.sparseResidency16Samples == VK_TRUE ? "+" : "-")) + "] " + "sparseResidency16Samples");
		Log::Message("[" + std::string((features.sparseResidencyAliased == VK_TRUE ? "+" : "-")) + "] " + "sparseResidencyAliased");
		Log::Message("[" + std::string((features.variableMultisampleRate == VK_TRUE ? "+" : "-")) + "] " + "variableMultisampleRate");
		Log::Message("[" + std::string((features.inheritedQueries == VK_TRUE ? "+" : "-")) + "] " + "inheritedQueries");

		Log::Message("");


		uint32_t amountQueueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFamilies, nullptr);
		VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[amountQueueFamilies];
		vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFamilies, queueFamilyProperties);

		for (size_t i = 0; i < amountQueueFamilies; i++)
		{
			Log::Message("===[ Queue Family " + std::to_string(i) + " ]===");
			Log::Message("Queue count:    " + std::to_string(queueFamilyProperties[i].queueCount));
			Log::Message("Graphics:       " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT != 0)));
			Log::Message("Compute:        " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT != 0)));
			Log::Message("Transfer:       " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT != 0)));
			Log::Message("PROTECTED:      " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_PROTECTED_BIT != 0)));
			Log::Message("SPARSE_BINDING: " + std::to_string((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_SPARSE_BINDING_BIT != 0)));
			Log::Message("");
		}

		Log::Message("");

		delete[] queueFamilyProperties;
	}
	void VulkanLogger::DisplayLayers()
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
			VulkanLogger::LogLayerInfo(layerProperties[i]);
		}
		Log::Message("");
		Log::Message("");
		delete[] layerProperties;
	}
}