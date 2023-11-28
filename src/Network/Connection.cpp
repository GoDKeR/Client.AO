#include "Connection.h"

namespace Network
{
	Connection::Connection()
	{
		incomingBuffer = std::make_unique<Utils::BinaryReader>();

		Int32 iResult;

		iResult = WSAStartup(MAKEWORD(2, 2), &mWSAData);
		if (iResult != 0)
		{
			printf("WSAStartup failed: %d\n", iResult);
			return;
		}

		/*hints.ai_family = AF_UNSPEC;
		hints.ai_socktype	= SOCK_STREAM;
		hints.ai_protocol	= IPPROTO_TCP;

		iResult = getaddrinfo(SERVER_ADDRESS.data(), std::to_string(SERVER_PORT).c_str(), &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return;
		}*/

		mServAddr.sin_family = AF_INET;
		mServAddr.sin_port = htons(SERVER_PORT);

		if (inet_pton(AF_INET, SERVER_ADDRESS.data(), &mServAddr.sin_addr) <= 0)
		{
			printf("Invalid address/ Address not supported. pipipi");
			return;
		}

		mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (mSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}
	}
	Connection::~Connection()
	{
		WSACleanup();
	}

	Int32 Connection::Connect()
	{
		Int32 iResult;

		iResult = connect(mSocket, (sockaddr*)&mServAddr, sizeof(mServAddr));
	}
}