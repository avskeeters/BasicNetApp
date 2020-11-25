#pragma once
#include "NetworkingLib/Shared.h"
#include "NetworkingLib/UDPSocket.h"

enum class SocketAddressFamily
{
	IPV4 = AF_INET,
	IPV6 = AF_INET6
};

class SocketUtilities
{
public:
	static bool InitWinSockets();
	static void Cleanup();
	static int GetLastError();
	static DWORD ShowLastError(LPCSTR lpszFunction, bool inWindow = false);
	static void ShowLastErrorAndExit(LPCSTR lpszFunction, bool inWindow = false);
	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily socketAddrFamily);
};