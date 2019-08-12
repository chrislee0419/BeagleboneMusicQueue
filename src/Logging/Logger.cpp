#include "Logger.h"

#include <iostream>
#include <fstream>
#include <mutex>

#ifdef DEBUG
// TODO: change to debug log level
#endif // DEBUG

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#ifdef X86
#define LOG_FILENAME "./log.txt"
#else
#define LOG_FILENAME "/root/musicQueue/logs/log"
#endif // X86

#define LOG_NAME_LENGTH 8

struct LoggerInternals
{
	static std::mutex _initMutex;
	static el::Configurations _conf;
	static bool _confInitialized;

	std::string _logName;
	el::Logger *_logger = nullptr;
};

std::mutex LoggerInternals::_initMutex;
bool LoggerInternals::_confInitialized = false;

void BackupOldLog(const char *filename, std::size_t size)
{
	std::ifstream existingLogFile;
	existingLogFile.open(filename, std::ios::binary);
	std::ofstream newLogFile;
	newLogFile.open(LOG_FILENAME "-old", std::ios::trunc | std::ios::binary);

	if (existingLogFile.is_open() && newLogFile.is_open())
	{
		std::streampos fileSize = static_cast<std::streampos>(size);

		char* logContents = new char[size];
		existingLogFile.read(logContents, fileSize);
		newLogFile.write(logContents, fileSize);
		delete[] logContents;
	}

	existingLogFile.close();
	newLogFile.close();
}

Logger::Logger(const std::string &logName)
{
	// configuration initialization (only needs to happen once during application lifetime)
	LoggerInternals::_initMutex.lock();
	if (!LoggerInternals::_confInitialized)
	{
		el::Configurations defaultConf;
		defaultConf.setToDefault();

		defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
		defaultConf.setGlobally(el::ConfigurationType::Filename, LOG_FILENAME);
		defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "4194304"); // 4 MiB
		defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
		defaultConf.setGlobally(el::ConfigurationType::LogFlushThreshold, "10");
		defaultConf.setGlobally(el::ConfigurationType::Format, "[%datetime{%Y/%M/%d %H:%m:%s.%g} | %logger | %level] %msg");

		el::Loggers::setDefaultConfigurations(defaultConf, true);

		el::Helpers::installPreRollOutCallback(BackupOldLog);

		LoggerInternals::_confInitialized = true;
	}
	LoggerInternals::_initMutex.unlock();

	// current logger initialization
	_internals = new LoggerInternals;
	_internals->_logName = logName;

	std::string standardizedLogName = logName;
	standardizedLogName.resize(LOG_NAME_LENGTH, '_');
	_internals->_logger = el::Loggers::getLogger(standardizedLogName);
}

Logger::~Logger()
{
	el::Loggers::unregisterLogger(_internals->_logName);
	delete _internals;
}

void Logger::Trace(const std::string &msg)
{
	_internals->_logger->trace(msg);
}

void Logger::Debug(const std::string& msg)
{
	_internals->_logger->debug(msg);
}

void Logger::Info(const std::string& msg)
{
	_internals->_logger->info(msg);
}

void Logger::Warning(const std::string& msg)
{
	_internals->_logger->warn(msg);
}

void Logger::Error(const std::string& msg)
{
	_internals->_logger->error(msg);
}