#pragma once
#define _USE_MATH_DEFINES
#include "UDPStream\UDPStream.hh"
using namespace SerraPlo;

class ClientProxy {
public:
	sockaddr address;
	std::string nick;
	ClientProxy() = default;
	ClientProxy(sockaddr addr, const std::string &str, int clientListSize) : address(addr), nick(str) {
		
	};
	~ClientProxy() = default;
};