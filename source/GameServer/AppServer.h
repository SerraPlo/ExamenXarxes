#pragma once
#include <SerraPloEngine/IApp.h>
#include "ClientProxy.h"
#include <map>
#pragma comment(lib, "SerraPloEngine.lib")

#define SERVER_PORT 5000
#define MAX_PLAYERS 2

class AppServer : public IApp {
	UDPStream dispatcher;
	std::map<uint64_t, ClientProxy*> clientList;
	float m_counterUpdate;
	float m_aliveCounter;

	explicit AppServer() : dispatcher(SERVER_PORT) {};
	AppServer(AppServer const&) = delete;
	void operator=(AppServer const&) = delete;
	~AppServer() { AppServer::Destroy(); }

	// Initialize everything related to game internals
	virtual void Init(void) override;
	void Send(void);
	void Receive(void);
	
public:
	static AppServer& Instance() {
		static AppServer instance;
		return instance;
	}
	// Where magic occurs, to be used to play the whole game
	virtual void Run(void) override;
	// Destroy screen list and set game running to false
	virtual void Destroy(void);
};