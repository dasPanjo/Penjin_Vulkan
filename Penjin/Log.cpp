#define _CRT_SECURE_NO_WARNINGS
#include "Log.h"
#include <iostream>
#include "File.h"

#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string

namespace Penjin {

	LogLevel Log::LogLevelConsole = LogLevel::Info;
	LogLevel Log::LogLevelFile = LogLevel::Error;

	void Log::Message(std::string message, bool supressFile) {
		std::string outputMessage = "[Info] " + message;
		LogText(outputMessage, LogLevel::Info, supressFile);
	}
	void Log::Error(std::string message, bool supressFile) {
		std::string outputMessage = "[Error] " + message;
		LogText(outputMessage, LogLevel::Error, supressFile);
	}
	void Log::Critical(std::string message, bool supressFile) {
		std::string outputMessage = "[Critical] " + message;
		LogText(outputMessage, LogLevel::Critical, supressFile);
	}
	void Log::LogText(std::string message, LogLevel logLevel, bool supressFile)
	{
		if (logLevel >= LogLevelConsole)
			std::cout << message << std::endl;
		if (!supressFile && logLevel >= LogLevelFile)
			File::Append("default.log", GetTimestamp() + message + "\n");

	}
	std::string Log::GetTimestamp()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "[%d.%m.%Y - %X]\t");
		return ss.str();
	}
}
