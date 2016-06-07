#pragma once
#include "SocketTools.h"

class SocketAddress
{
	sockaddr_in m_address {0};
public:
	SocketAddress() = default;
	SocketAddress(const std::string &inString);
	SocketAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint16_t port);
	SocketAddress(const SocketAddress &socketAdress);
	~SocketAddress() = default;

	int setAddress(const std::string &inString);
	void setAddress(const sockaddr &a);
	sockaddr* getSockaddrAddress();

	SocketAddress& operator=(const SocketAddress& other) = default;
	friend std::ostream& operator<<(std::ostream& os, const SocketAddress &sa);
	bool operator==(SocketAddress& sa) const;
};

