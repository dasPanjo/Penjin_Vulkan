#include <iostream>
#include "Application3D.h"
#include <vulkan/vulkan_core.h>

#define _SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING

int main() {
	int result = Application3D::Run("Game Logic", VK_MAKE_VERSION(0, 0, 0), 1280, 720);
	if (result == EXIT_FAILURE)
		std::getchar();
	return result;
}