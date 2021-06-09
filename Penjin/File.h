#pragma once
#include <string>

namespace Penjin {
	class File
	{
	public:
		static bool Exists(std::string filename);
		static bool Exists(const char* filename) { return Exists(std::string(filename)); }

		static bool Write(std::string filename, std::string text, int maxFileSize = -1);
		static bool Write(std::string filename, const char* text, int maxFileSize = -1) { return Write(filename, std::string(text), maxFileSize); }
		static bool Write(const char* filename, std::string text, int maxFileSize = -1) { return Write(std::string(filename), text, maxFileSize); }
		static bool Write(const char* filename, const char* text, int maxFileSize = -1) { return Write(std::string(filename), std::string(text), maxFileSize); }

		static bool Append(std::string filename, std::string text, int maxFileSize = -1);
		static bool Append(std::string filename, const char* text, int maxFileSize = -1) { return Append(filename, std::string(text), maxFileSize); }
		static bool Append(const char* filename, std::string text, int maxFileSize = -1) { return Append(std::string(filename), text, maxFileSize); }
		static bool Append(const char* filename, const char* text, int maxFileSize = -1) { return Append(std::string(filename), std::string(text), maxFileSize); }

		static std::string Read(std::string filename);
		static std::string Read(const char* filename) { return Read(std::string(filename)); }
	};
}

