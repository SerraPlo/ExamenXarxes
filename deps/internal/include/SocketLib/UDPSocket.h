#pragma once
#include "GenericSocket.h"

class UDPSocket : public GenericSocket
{
public:
	explicit UDPSocket();
	UDPSocket(const UDPSocket& other) = default;
	~UDPSocket() = default;

	int SendTo(const char* data, int lenData, SocketAddress& to) const;
	int SendTo(const std::string &data, SocketAddress &to) const;

	int ReceiveFrom(char* data, int lenData, SocketAddress& from) const;
	int ReceiveFrom(std::string  &data, SocketAddress & from) const;

	UDPSocket& operator=(const UDPSocket& other) = default;
};

