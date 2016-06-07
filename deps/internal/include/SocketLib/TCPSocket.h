#pragma once
#include "GenericSocket.h"
#include <memory>

class TCPSocket : public GenericSocket
{
	bool m_isDelayDisabled {false};
public:
	explicit TCPSocket();
	explicit TCPSocket(SOCKET s);
	~TCPSocket();

	void Listen(int backlog) const;
	void Connect(SocketAddress& addr) const;
	std::shared_ptr<TCPSocket> Accept(SocketAddress& from) const;

	int Send(const char *data) const;
	int Send(const std::string &data) const;

	int Receive(char *data, int lenData) const;
	int Receive(std::string &data) const;

	//CAUTION: doesnt work correctly yet
	void DisableDelay(); //send different message sizes without merge
};

