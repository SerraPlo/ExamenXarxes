#pragma once
#include "SocketAddress.h"

#if CUR_PLATFORM != PLATFORM_WINDOWS
	typedef int SOCKET;
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
	#define NO_ERROR				(0L)
	#define closesocket(sockdep)	close(sockdep)
	#define SD_BOTH					(0x02)
#endif

#define MAX_BYTES 1300 // max bytes of data to be sent/received

class GenericSocket
{
protected:
	SOCKET m_socket {0};
	unsigned m_type {0};
	u_long m_isNonBlocking {0};
public:
	explicit GenericSocket(int type);
	GenericSocket(const GenericSocket& other) = default;
	virtual ~GenericSocket();

	void Bind(SocketAddress& address) const;
	void NonBlocking(bool isNonBlocking);

	GenericSocket& operator=(const GenericSocket& other) = default;
};