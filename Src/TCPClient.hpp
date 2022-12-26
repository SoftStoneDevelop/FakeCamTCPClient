#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#endif

#include "MemoryOwner.hpp"
#include "MemoryOwnerFactory.hpp"

namespace FakeCamClient
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
        void close_();

		bool sendCommand(
			const char* data, const int dataSize, 
			ArrayPool::MemoryOwner<char>& responce
		);

		const std::string& getPort() const noexcept { return port_; }
		const std::string& getHost() const noexcept { return host_; }

	private:

		std::string host_;
		std::string port_;

#if defined(_WIN32)
        SOCKET connectSocket_ = INVALID_SOCKET;
#elif defined(__linux__)
        int connectSocket_ = -1;
#endif

		bool sendShutdown_ = false;
		bool isConnected_ = false;

		ArrayPool::MemoryOwner<char> readBuffer_;
		ArrayPool::MemoryOwnerFactory<char>& factory_;
	};
}//namespace ClientMJPEG
