#pragma once
#include <string>

#define LOG(message, ...) { \
	char fmsg[256]; \
	snprintf(fmsg, sizeof(fmsg), message, __VA_ARGS__); \
	Logger::LogMessage(fmsg); \
}

#define LOG_WARNING(message, ...) { \
	char fmsg[256]; \
	snprintf(fmsg, sizeof(fmsg), message, __VA_ARGS__); \
	Logger::LogWarning(fmsg); \
}

#define LOG_ERROR(message, ...) { \
	char fmsg[256]; \
	snprintf(fmsg, sizeof(fmsg), message, __VA_ARGS__); \
	Logger::LogError(fmsg); \
}


/**
* Centralized logging class
* NOTE: All text output should be through this class
*/
class Logger
{
public:
	static void LogMessage(std::string msg);
	static void LogWarning(std::string msg);
	static void LogError(std::string msg);
};

