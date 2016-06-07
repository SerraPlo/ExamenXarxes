#include "UDPSocket.h"

UDPSocket::UDPSocket() : GenericSocket(SOCK_DGRAM) {}

int UDPSocket::SendTo(const char* data, int lenData, SocketAddress &to) const {
	auto bytesSent = sendto(m_socket, data, lenData, 0, to.getSockaddrAddress(), sizeof(sockaddr_in));
	if (bytesSent == SOCKET_ERROR) SocketTools::SL_THROW_ERROR("Data could not be sent.");
	return bytesSent;
}

int UDPSocket::SendTo(const std::string &data, SocketAddress &to) const {
	auto bytesSent = sendto(m_socket, data.c_str(), int(data.length()), 0, to.getSockaddrAddress(), sizeof(sockaddr_in));
	if (bytesSent == SOCKET_ERROR) SocketTools::SL_THROW_ERROR("Data could not be sent.");
	return bytesSent;
}

int UDPSocket::ReceiveFrom(char* data, int lenData, SocketAddress &from) const {
	memset(data, NULL, MAX_BYTES);

	sockaddr tempAddr;
	int addrSize = sizeof(sockaddr_in);
	auto bytesReceived = recvfrom(m_socket, data, lenData, 0, &tempAddr, &addrSize);
	from.setAddress(tempAddr);

	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::SL_THROW_ERROR("Data could not be received.");

	/*if (bytesReceived == MAX_BYTES)
		for (int i = 0; i < MAX_BYTES; ++i) 
			if (cData[i] == NULL) { bytesReceived = i; break; } //real bytes received*/

	return bytesReceived;
}

int UDPSocket::ReceiveFrom(std::string &data, SocketAddress &from) const {
	sockaddr tempAddr;
	int addrSize = sizeof(sockaddr_in);
	const int sizeData = int(data.length());
	char cData[MAX_BYTES];
	auto bytesReceived = recvfrom(m_socket, cData, MAX_BYTES, 0, &tempAddr, &addrSize);
	if (bytesReceived >= 0) {
		from.setAddress(tempAddr);
		data = std::string(cData).substr(0, bytesReceived);
	}
	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::SL_THROW_ERROR("Data could not be received.");

	return bytesReceived;
}
