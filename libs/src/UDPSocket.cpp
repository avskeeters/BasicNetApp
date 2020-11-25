#include "NetworkingLib/UDPSocket.h"
#include "NetworkingLib/Shared.h"
#include "NetworkingLib/SocketAddress.h"
#include "NetworkingLib/SocketUtilities.h"

class UDPSocket::Impl
{
public:
	Impl(SOCKET initializedSocket) : socket{initializedSocket} {}
	SOCKET socket;
};

int UDPSocket::Bind( SocketAddress const& toAddress )
{
	auto const errorCode = bind(pImpl->socket, toAddress.GetAsSockAddrPtr(), static_cast<int>(toAddress.Size()));
	if(errorCode != 0)
	{
		SocketUtilities::ShowLastError("UDPSocket::Bind");
		return SocketUtilities::GetLastError();
	}
	return NO_ERROR;
}

UDPSocket::~UDPSocket()
{
	closesocket(pImpl->socket);
}

int UDPSocket::SendTo( void const* dataToSend, int const dataLength, SocketAddress const& toAddress )
{
	auto const numBytesSent = sendto(pImpl->socket, static_cast<char const*>(dataToSend),
		dataLength,
		0,
		toAddress.GetAsSockAddrPtr(),
		static_cast<int>(toAddress.Size()));
	
	if(numBytesSent >= 0)
	{
		return numBytesSent;
	}
	
	// Return errors as negative number. Non-negative indicates that nothing went wrong with sendto explicitly
	SocketUtilities::ShowLastError("UDPSocket::SendTo");
	return -SocketUtilities::GetLastError();
}

int UDPSocket::ReceiveFrom( void* dataBuffer, int maxDataLength, SocketAddress& fromAddress )
{
	auto fromAddressLength = static_cast<int>(fromAddress.Size()); // recvfromm writes address received from to fromAddress

	auto const numBytesRead = recvfrom(pImpl->socket,
		static_cast<char*>(dataBuffer),
		maxDataLength,
		0,
		fromAddress.GetAsSockAddrPtr(),
		&fromAddressLength);

	if(numBytesRead >= 0)
	{
		return numBytesRead;
	}

	auto const errorCode = SocketUtilities::GetLastError();

	if(errorCode == WSAEWOULDBLOCK)
	{
		return 0; // Socket running in asynchronous mode and no data to read. WOULD'VE blocked if had been running synchronously.
	}
	
	if(errorCode == WSAECONNRESET)
	{
		// Can happen if client closed and we haven't disconnected yet.
		// Message being sent back saying that the port on that pc is closed
		return -WSAECONNRESET;
	}
	
	SocketUtilities::ShowLastError("UDPSocket::ReceiveFrom");
	return -errorCode;
}

UDPSocket::UDPSocket( void* hardwareSpecificSocket )
	: pImpl{ std::make_unique<Impl>(*static_cast<SOCKET*>(hardwareSpecificSocket)) }
{}
