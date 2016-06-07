#include "GenericSocket.h"

GenericSocket::GenericSocket(int type) : m_type(type) {
	if ((m_socket = socket(AF_INET, type, IPPROTO_IP)) == INVALID_SOCKET)
		SocketTools::SL_THROW_ERROR("Socket could not be initialised.");
}

void GenericSocket::Bind(SocketAddress & address) const {
	if (bind(m_socket, address.getSockaddrAddress(), sizeof(sockaddr)) == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("Address could not be binded to socket.");
}

void GenericSocket::NonBlocking(bool nb) {
	m_isNonBlocking = nb ? 1 : 0;
#if CUR_PLATFORM == PLATFORM_WINDOWS
	if (ioctlsocket(m_socket, FIONBIO, &m_isNonBlocking) == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("Socket could not be changed to non-blocking mode.");
#else
	if (fcntl(handle, F_SETFL, O_NONBLOCK, (int)nonBlocking) == -1)
		SocketTools::ThrowError("Socket could not be changed to non-blocking mode.");
#endif
}

GenericSocket::~GenericSocket() {
	if (closesocket(m_socket) != NO_ERROR)
		SocketTools::SL_THROW_ERROR("Socket could not be destroyed.");
}
