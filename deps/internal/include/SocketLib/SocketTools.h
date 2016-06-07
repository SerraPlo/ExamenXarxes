#pragma once
#include <exception>
#include <string>

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define CUR_PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define CUR_PLATFORM PLATFORM_MAC
#else
#define CUR_PLATFORM PLATFORM_UNIX
#endif

#if CUR_PLATFORM == PLATFORM_WINDOWS // Windows x64 and x86
	#define WIN32_LEAN_AND_MEAN
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501  // Windows XP
	#endif
	#include <Ws2tcpip.h>
	#include <winsock2.h>
#else // Linux and Mac
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <fcntl.h>
	#include <netdb.h>  // getaddrinfo() and freeaddrinfo()
	#include <unistd.h> // close()
#endif

namespace SocketTools
{
#define SL_THROW_ERROR(message) ShowError(message, __FILE__, __LINE__)
	inline void ShowError(std::string &&message, const std::string& fileName, int line) {
#if CUR_PLATFORM == PLATFORM_WINDOWS
		message.append("\nFile: " + fileName + "\nLine: " + std::to_string(line) + "\nWin32 error code: " + std::to_string(WSAGetLastError()));
#endif
		throw std::exception(message.c_str());
	};
	inline void BuildLibrary(void) {
#if CUR_PLATFORM == PLATFORM_WINDOWS
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) SL_THROW_ERROR("WSAStartup could not initialise WinSock library.");
#endif
	};
	inline void UnloadLibrary(void) {
#if CUR_PLATFORM == PLATFORM_WINDOWS
		if (WSACleanup() != NO_ERROR) SL_THROW_ERROR("WSACleanup could not clear unload WinSock library.");
#endif
	};
};

