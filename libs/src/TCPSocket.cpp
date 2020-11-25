#include "NetworkingLib/Shared.h"

class TCPSocket::Impl
{
public:
	Impl(SOCKET initializedSocket) : socket{ initializedSocket } {}
	SOCKET socket;
};

TCPSocket::~TCPSocket()
{
	closesocket(pImpl->socket);
}

TCPSocket::TCPSocket(void* hardwareSpecificSocket)
	: pImpl{ std::make_unique<Impl>( *static_cast<SOCKET*>(hardwareSpecificSocket) ) }
{}

int TCPSocket::Connect( SocketAddress const& toAddress )
{
	auto const errorCode = connect(pImpl->socket,
		toAddress.GetAsSockAddrPtr(),
		static_cast<int>(toAddress.Size()));

	if(errorCode < 0)
	{
		SocketUtilities::ShowLastError("TCPSocket::Connect");
		return -SocketUtilities::GetLastErrorCode(); // returning errors as negative again
	}
	return NO_ERROR;
}

int TCPSocket::Bind( SocketAddress const& toAddress )
{
	auto const errorCode = bind(pImpl->socket, toAddress.GetAsSockAddrPtr(), static_cast<int>(toAddress.Size()));
	if(errorCode != 0)
	{
		SocketUtilities::ShowLastError("TCPSocket::Bind");
		return SocketUtilities::GetLastErrorCode();
	}
	return NO_ERROR;
}

int TCPSocket::Listen( int maxBackLog )
{
	auto const errorCode = listen(pImpl->socket, maxBackLog);
	if(errorCode < 0)
	{
		SocketUtilities::ShowLastError("TCPSocket::Listen");
		return -SocketUtilities::GetLastErrorCode();
	}
	return NO_ERROR;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept( SocketAddress& fromAddress )
{
	auto addressSize = static_cast<int>(fromAddress.Size());
	SOCKET newSocket = accept(pImpl->socket, fromAddress.GetAsSockAddrPtr(), &addressSize);
	if(newSocket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(&newSocket));
	}
	SocketUtilities::ShowLastError("TCPSocket::Accept");
	return nullptr;
}

int32_t TCPSocket::Send( void const* dataToSend, size_t dataLength )
{
	auto const numBytesSent = send(pImpl->socket, static_cast<char const*>(dataToSend), static_cast<int>(dataLength), 0);
	if(numBytesSent < 0)
	{
		SocketUtilities::ShowLastError("TCPSocket::Send");
		return -SocketUtilities::GetLastErrorCode(); // Error code negative again because sent should be non-negative
	}
	return numBytesSent;
}

int32_t TCPSocket::Receive( void* dataBuffer, size_t maxDataLength )
{
	auto const numBytesReceived = recv(pImpl->socket, static_cast<char*>(dataBuffer), static_cast<int>(maxDataLength), 0);
	if(numBytesReceived < 0)
	{
		SocketUtilities::ShowLastError("TCPSocket::Receive");
		return -SocketUtilities::GetLastErrorCode();
	}
	return numBytesReceived;
}






