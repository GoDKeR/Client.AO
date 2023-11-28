#pragma once

#include <Utils/BinaryReader.h>
#include <Defines.h>

#include <WinSock2.h>
#include <ws2tcpip.h>

namespace Network
{
	class Connection
	{
	private:

		static constexpr std::string_view	SERVER_ADDRESS	= "127.0.0.1";
		static constexpr Int32				SERVER_PORT		= 7666;

		UPtr<Utils::BinaryReader> incomingBuffer	= nullptr;
		//BinaryWriter outgoingBuffer;

		WSADATA mWSAData;
		SOCKET	mSocket		= INVALID_SOCKET;

		//addrinfo* result = NULL, * ptr = NULL, hints = { 0 };
		sockaddr_in mServAddr = { 0 };

		Int32 Connect();
	public:
		Connection();
		~Connection();
	};
}