#include "Main.h"

#include <mutex>
#include <condition_variable>

#include "Logging/Logger.h"

static Logger *mainLogger = nullptr;

static std::condition_variable shutdownCV;
static std::mutex shutdownMutex;
static bool triggerShutdown = false;

// Start up all modules that will be immediately used by this application.
// Returns: true, if modules have initialized successfully; false, otherwise.
static bool InitializeModules()
{
	mainLogger->Info("Application started. Initalizing modules...");

	// TODO: initialize all modules

	return true;
}

void ShutdownApplication()
{
	mainLogger->Info("Request to shutdown application received");

	std::unique_lock<std::mutex> lk(shutdownMutex, std::try_to_lock);

	if (lk.owns_lock())
	{
		triggerShutdown = true;
		lk.unlock();
		shutdownCV.notify_one();
	}
	else
	{
		mainLogger->Warning("Unable to send shutdown request to main thread (still in initialization process?)");
	}
}

int main()
{
	std::unique_lock<std::mutex> lk(shutdownMutex);

	mainLogger = new Logger("main");

	if (InitializeModules())
	{
		mainLogger->Info("All modules initialized successfully.");

		shutdownCV.wait(lk, []() { return triggerShutdown; });
		lk.unlock();

		// TODO: perform clean-up for all modules
		delete mainLogger;
	}
	else
	{
		mainLogger->Error("Unable to initialize all modules successfully. Shutting down application...");

		delete mainLogger;
		return 1;
	}

	return 0;
}
