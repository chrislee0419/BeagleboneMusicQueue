#pragma once

#include <thread>

#include "../ModuleInterface.h"

class Server : public ModuleInterface
{
private:
	std::thread *_sendThread;
	std::thread *_recvThread;

	Server();
	~Server();
}
