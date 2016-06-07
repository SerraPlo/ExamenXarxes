#pragma once
#define _USE_MATH_DEFINES
#include "UDPStream\UDPStream.hh"
#include <glm/vec2.hpp>
using namespace SerraPlo;

class ClientProxy {
public:
	sockaddr address;
	std::string nick;
	glm::ivec2 position;
	ClientProxy() = default;
	ClientProxy(sockaddr addr, const std::string &str, uint32_t clientListSize) : address(addr), nick(str) {
		position = { 100, clientListSize * 100 };
	};
	~ClientProxy() = default;
};