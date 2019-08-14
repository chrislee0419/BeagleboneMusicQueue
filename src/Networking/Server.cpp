#include "Server.h"

#include <string>

#include "Logging/Logger.h"

namespace {
	const std::string CMD_PING        = "ping";
	const std::string CMD_VOLUME_UP   = "volup";
	const std::string CMD_VOLUME_DOWN = "voldown";
	const std::string CMD_PLAY        = "play";
	const std::string CMD_PAUSE       = "pause";
	const std::string CMD_SKIP        = "skip";
	const std::string CMD_ADD_SONG    = "addsong=%s";
	const std::string CMD_REMOVE_SONG = "rmsong=%s,%d";
	const std::string CMD_REPEAT_SONG = "repeat=%d";
	const std::string CMD_SET_VOL     = "vol=%d";
	
	const int CMD_ADD_SONG_NUM_PARAM    = 1;
	const int CMD_REMOVE_SONG_NUM_PARAM = 2;
	const int CMD_REPEAT_SONG_NUM_PARAM = 1;
	const int CMD_SET_VOL_NUM_PARAM     = 1;

	const int VOLUME_INCREMENT = 5;
};

Server::Server()
{
	_sendThread = new std::thread();
	_recvThread = new std::thread();
}

Server::~Server()
{
	_sendThread.join();
	_recvThread.join();

	delete _sendThread;
	delete _recvThread;
}

ModuleInterface &Server::GetInstance()
{
	static Server instance;
	return instance;
}
