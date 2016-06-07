#include "SocketAddress.h"

SocketAddress::SocketAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint16_t port) {
	m_address.sin_addr.S_un.S_un_b = { byte1 ,byte2, byte3, byte4 };
	m_address.sin_family = AF_INET;
	m_address.sin_port = port;
	for (auto i = 0; i < 8; ++i) m_address.sin_zero[i] = 0;
}

SocketAddress::SocketAddress(const std::string & inString) {
	setAddress(inString);
}

SocketAddress::SocketAddress(const SocketAddress &socketAddress) : 
	SocketAddress(socketAddress.m_address.sin_addr.S_un.S_un_b.s_b1, 
				  socketAddress.m_address.sin_addr.S_un.S_un_b.s_b2,
				  socketAddress.m_address.sin_addr.S_un.S_un_b.s_b3,
				  socketAddress.m_address.sin_addr.S_un.S_un_b.s_b4,
				  socketAddress.m_address.sin_port) {}

int SocketAddress::setAddress(const std::string & inString) {
	//find_last_of returns the last position of the character especified if exists, else returns string::npos
	auto pos = inString.find_last_of(':'); 
	std::string host, service;

	//npos is a constant with the highest possible value that can reach a variable of type size_t.
	//size_t is a numeric type with the sufficient number of bits to represent the largest number of array size that can exist.
	//npos is -1, because as size_t is an unsigned type, -1 is the greater value that can be represented (when you turn in binary)
	if (pos != std::string::npos) {
		host = inString.substr(0, pos);
		service = inString.substr(pos + 1);
	} else {
		host = inString;
		service = "0"; //0 is default port
	}

	//Forces all memory corresponding to hint to be 0, in order to avoid misunderstandings at getaddrinfo.
	//hint's function will be filtering the results that returns getaddrinfor 
	//In this case, will just return those addresses which are IPv4.
	addrinfo hint;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	//result will store a list of IP:port adresses lista de las direcciones which matches search criteria
	addrinfo* result;
	auto error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
	if (error != 0 && result != nullptr) {
		freeaddrinfo(result);
		SocketTools::SL_THROW_ERROR("List of IP:port addresses could not be stored.");
	}

	//Go through addresses of result in search of a valid one
	while (!result->ai_addr && result->ai_next) 
		result = result->ai_next;
	if (!result->ai_addr) {
		freeaddrinfo(result);
		SocketTools::SL_THROW_ERROR("Non valid address found.");
	}

	//memcpy copies each byte of the valid address to the member address of the class
	memcpy(&m_address, result->ai_addr, sizeof(sockaddr_in));
	freeaddrinfo(result);

	return 0;
}

void SocketAddress::setAddress(const sockaddr &a) {
	memcpy(&m_address, &a, sizeof(sockaddr_in));
}

sockaddr* SocketAddress::getSockaddrAddress() {
	return static_cast<sockaddr*>(static_cast<void*>(&m_address));
}

std::ostream& operator<<(std::ostream& os, const SocketAddress &sa) {
	return os << int(sa.m_address.sin_addr.S_un.S_un_b.s_b1) << "." << int(sa.m_address.sin_addr.S_un.S_un_b.s_b2) << "." << int(sa.m_address.sin_addr.S_un.S_un_b.s_b3) << "." << int(sa.m_address.sin_addr.S_un.S_un_b.s_b4) << ":" << htons(sa.m_address.sin_port);
}

bool SocketAddress::operator==(SocketAddress & sa) const {
	if (m_address.sin_port != sa.m_address.sin_port || m_address.sin_addr.S_un.S_addr != sa.m_address.sin_addr.S_un.S_addr) return false;
	return true;
}
