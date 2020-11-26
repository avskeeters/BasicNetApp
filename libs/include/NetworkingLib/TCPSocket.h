#pragma once
#include <memory>

class SocketAddress;

class TCPSocket
{
public:
	~TCPSocket();
	int Connect(SocketAddress const& toAddress);
	int Bind(SocketAddress const& toAddress);
	int Listen(int maxBackLog = 32);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& fromAddress);
	int32_t Send(void const* dataToSend, size_t dataLength);
	int32_t Receive(void* dataBuffer, size_t maxDataLength);
private:
	TCPSocket(void* hardwareSpecificSocket);
	void* GetRawSocketPtr();
	void const* GetRawSocketPtr() const;
	friend class SocketUtilities;
	class Impl;
	std::unique_ptr<Impl> pImpl;
};
using TCPSocketPtr = std::shared_ptr<TCPSocket>;