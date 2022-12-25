#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <mutex>
#include <stdlib.h>
#include <stdio.h>
#include <future>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "MemoryOwner.hpp"
#include "MemoryOwnerFactory.hpp"

namespace FakeCamTCPClient
{
	class TCPClient
	{
	public:
		TCPClient(
			std::string&& host,
			std::string&& port,
			ArrayPool::MemoryOwnerFactory<char>& factory
		);
		~TCPClient();

		TCPClient(const TCPClient& other) = delete;
		TCPClient(TCPClient&& other) = delete;

		TCPClient& operator=(TCPClient&& other) = delete;
		TCPClient& operator=(const TCPClient& other) = delete;

		bool Connect();
		void close();

		bool sendCommand(
			const char* data, const int dataSize, 
			ArrayPool::MemoryOwner<char>& responce
		);

		const std::string& getPort() const noexcept { return port_; }
		const std::string& getHost() const noexcept { return host_; }

	private:

		std::string host_;
		std::string port_;
		SOCKET connectSocket_ = INVALID_SOCKET;

		bool sendShutdown_ = false;
		bool isConnected_ = false;

		ArrayPool::MemoryOwner<char> readBuffer_;
		ArrayPool::MemoryOwnerFactory<char>& factory_;
	};
}//namespace ClientMJPEG