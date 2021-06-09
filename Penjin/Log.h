#pragma once
#include <string>
#include <strstream>
#include "LogLevel.h"

namespace Penjin {
	class Log
	{
	public:
		static LogLevel LogLevelConsole;
		static LogLevel LogLevelFile;

		static void Message(std::string message, bool supressFile = false);
		static void Message(const char* message, bool supressFile = false) { Message(std::string(message), supressFile); }

		static void Error(std::string message, bool supressFile = false);
		static void Error(const char* message, bool supressFile = false) { Error(std::string(message), supressFile); }

		static void Critical(std::string message, bool supressFile = false);
		static void Critical(const char* message, bool supressFile = false) { Error(std::string(message), supressFile); }

		struct TextStream : public std::strstream {
			friend std::strstream& operator<<(std::strstream& cout, TextStream& obj)
			{
				Message(cout.str());
				return cout;
			}
		};

		static TextStream Text;

	private:
		static void LogText(std::string message, LogLevel logLevel, bool supressFile = false);
		static std::string GetTimestamp();

	};
}