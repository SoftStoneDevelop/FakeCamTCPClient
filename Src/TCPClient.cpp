#pragma once

#include "TCPClient.hpp"
#include <mutex>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>

namespace FakeCamTCPClient
{
	TCPClient::TCPClient(
		std::string&& host,
		std::string&& port,
		ArrayPool::MemoryOwnerFactory<char>& factory
	) : host_{ host }, port_{ port }, factory_{ factory }
	{
		readBuffer_ = factory_.rentMemory(4000);
	}

	TCPClient::~TCPClient()
	{
		close();
	}

	void TCPClient::close()
	{
		if (isConnected_)
		{
			closesocket(connectSocket_);
			WSACleanup();
			isConnected_ = false;
		}
	}

	bool TCPClient::Connect()
	{
		printf("Connect..\n");
		if (isConnected_)
		{
			printf("already connected\n");
			return false;
		}
		int iResult;

		// Initialize Winsock
		WSADATA wsaData;
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			printf("WSAStartup failed with error: %d\n", iResult);
			return false;
		}

		addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		addrinfo* result = nullptr;
		// Resolve the server address and port
		iResult = getaddrinfo(host_.c_str(), port_.c_str(), &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		addrinfo* ptr = nullptr;
		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			// Create a SOCKET for connecting to server
			connectSocket_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (connectSocket_ == INVALID_SOCKET)
			{
				WSACleanup();
				freeaddrinfo(result);
				printf("socket failed with error: %d\n",WSAGetLastError());
				return false;
			}

			// Connect to server.
			iResult = connect(connectSocket_, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				closesocket(connectSocket_);
				connectSocket_ = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (connectSocket_ == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			WSACleanup();
			return false;
		}

		isConnected_ = true;
		printf("connected\n");
		return isConnected_;
	}

	bool TCPClient::sendCommand(
		const char* data,
		const int dataSize,
		ArrayPool::MemoryOwner<char>& responce
	)
	{
		int iResult = send(connectSocket_, data, dataSize, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket_);
			WSACleanup();
			return false;
		}

		iResult = send(connectSocket_, "\n", 1, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket_);
			WSACleanup();
			return false;
		}

		bool findNewline = false;
		int readSize = 0;
		while (!findNewline)
		{
			auto recived = recv(connectSocket_, readBuffer_.data(), readBuffer_.size(), 0);
			if (recived < 0)
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(connectSocket_);
				WSACleanup();

				return false;
			}
			else if(recived == 0)
			{
				printf("connection close");
				closesocket(connectSocket_);
				WSACleanup();

				return false;
			}

			if (responce.size() - (readSize + recived) <= 0)
			{
				auto newResponce = factory_.rentMemory((readSize + recived) * 2);
				std::copy(responce.data(), responce.data() + readSize, newResponce.data());
				responce = std::move(newResponce);
			}
			
			std::copy(readBuffer_.data(), readBuffer_.data() + recived, responce.data() + readSize);
			if (std::find(responce.data() + readSize, responce.data() + readSize + recived, '\n') != responce.data() + readSize + recived)
			{
				readSize += recived;
				responce.data()[readSize] = '\0';
				findNewline = true;
			}
		}

		return true;
	}

}//namespace FakeCamTCPClient