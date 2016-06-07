#include "TCPSocket.h"

TCPSocket::TCPSocket() : GenericSocket(SOCK_STREAM) {}

TCPSocket::TCPSocket(SOCKET s) : GenericSocket(SOCK_STREAM) {
	m_socket = s;
}

TCPSocket::~TCPSocket(){
	if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("TCPSocket: error shuting down.");
}

void TCPSocket::Listen(int backlog) const {
	if (listen(m_socket, backlog) == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("TCPSocket: error while listening from backlog.");
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress & from) const {
	sockaddr sa;
	int sizeSa = sizeof(sockaddr);
	SOCKET newSocket = accept(m_socket, &sa, &sizeSa);
	from.setAddress(sa);
	return std::make_shared<TCPSocket>(newSocket);
}

void TCPSocket::Connect(SocketAddress & addr) const {
	if (connect(m_socket, addr.getSockaddrAddress(), sizeof(sockaddr)) == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("TCPSocket: cannot connect to addr.");
}

int TCPSocket::Send(const char *data) const {
	int lenData { MAX_BYTES };
	if (m_isDelayDisabled) lenData = int(strlen(data));

	auto bytesSent = send(m_socket, data, lenData, 0);
	if (bytesSent == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("TCPSocket: can't send data.");
	return bytesSent;
}

int TCPSocket::Send(const std::string &data) const {
	int lenData { MAX_BYTES };
	if (m_isDelayDisabled) lenData = int(data.length());

	auto bytesSent = send(m_socket, data.c_str(), lenData, 0);
	if (bytesSent == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("TCPSocket: can't send data.");
	return bytesSent;
}

int TCPSocket::Receive(char *data, int lenData) const {
	memset(data, NULL, lenData);

	int bytesReceived = recv(m_socket, data, lenData, 0); //blocks execution until data arrives if blocking socket mode
	
	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::SL_THROW_ERROR("TCPSocket: error receiving data.");
	else if (bytesReceived == 0) SocketTools::SL_THROW_ERROR("TCPSocket: Connection closed.");

	if (bytesReceived > 0) printf("RECEIVE { data: %s, bytes: %d }\n", data, bytesReceived);
	
	return bytesReceived;
}

int TCPSocket::Receive(std::string &data) const {
	auto cData = const_cast<char*>(data.c_str());
	int bytesReceived = recv(m_socket, cData, MAX_BYTES, 0); //blocks execution until data arrives if blocking socket mode
	data = cData;

	if (bytesReceived == SOCKET_ERROR && !m_isNonBlocking) SocketTools::SL_THROW_ERROR("TCPSocket: error receiving data.");
	else if (bytesReceived == 0) SocketTools::SL_THROW_ERROR("TCPSocket: Connection closed.");

	return bytesReceived;
}

//TODO: cross-platform
void TCPSocket::DisableDelay() { //CAUTION: doesnt work correctly yet
	m_isDelayDisabled = true;
	char flag = 1;
	if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) == SOCKET_ERROR)
		SocketTools::SL_THROW_ERROR("TCPSocket: TCP_NODELAY could not be enabled.");
}
