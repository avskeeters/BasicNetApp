#pragma once
#include "NetworkingLib/Shared.h"
#include <vector>

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
	static int GetLastErrorCode();
	static DWORD ShowLastError(LPCSTR lpszFunction, bool inWindow = false);
	static void ShowLastErrorAndExit(LPCSTR lpszFunction, bool inWindow = false);
	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily socketAddrFamily);
	static TCPSocketPtr CreateTCPSocket(SocketAddressFamily socketAddrFamily);
	static int Select(std::vector<TCPSocketPtr> const* checkForReadable,
					  std::vector<TCPSocketPtr> *outputReadable,
		              std::vector<TCPSocketPtr> const* checkForWritable,
					  std::vector<TCPSocketPtr>* outputWritable,
		              std::vector<TCPSocketPtr> const* checkForExceptions,
		              std::vector<TCPSocketPtr>* outputExceptions); // Wrap native select function to work with our vector of ptrs vs native socket sets
private:
	// Functions of multiple TCP sockets
	static fd_set* VectorToFDSet(fd_set& setToFill, std::vector<TCPSocketPtr> const* sockets);
	static void FDSetToVector(std::vector<TCPSocketPtr>* vectorToFill, std::vector<TCPSocketPtr> const* vectorToPullFrom, fd_set const& setToFill);
};