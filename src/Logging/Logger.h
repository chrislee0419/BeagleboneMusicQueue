#pragma once

#include <string>

struct LoggerInternals;

class Logger
{
private:
	LoggerInternals *_internals;

public:
	Logger(const std::string &logName);
	~Logger();

	void Trace(const std::string &msg);
	void Debug(const std::string &msg);
	void Info(const std::string &msg);
	void Warning(const std::string &msg);
	void Error(const std::string &msg);
};

