#define _CRT_SECURE_NO_WARNINGS
#include "File.h"
#include <fstream>
#include "Log.h"
#include <iostream>
#include <Windows.h>

namespace Penjin {
	bool File::Exists(std::string filename) {
		std::ifstream f(filename);
		return f.good();
	}

	bool File::Write(std::string filename, std::string text, int maxFileSize)
	{
		std::ofstream filestream(filename, std::ios::out);
		if (filestream.fail()) {
			Log::Error("File::Write error: file '" + filename + "' not found!", true);
			return false;
		}

		filestream << text;
		filestream.close();
		return true;
	}

	bool File::Append(std::string filename, std::string text, int maxFileSize)
	{
		std::ofstream filestream(filename, std::ios::app);
		if (filestream.fail()) {
			Log::Error("File::Write error: file '" + filename + "' not found!", true);
			return false;
		}
		filestream << text;
		filestream.close();
		return true;
	}

	std::string File::Read(std::string filename)
	{
		std::ifstream filestream(filename, std::ios::binary);
		if (filestream.fail()) {
			Log::Error("File::Read error: file '" + std::string(filename) + "' not found!");
			return "";
		}
		std::string contents((std::istreambuf_iterator<char>(filestream)),
			(std::istreambuf_iterator<char>()));
		return contents;
	}

}
