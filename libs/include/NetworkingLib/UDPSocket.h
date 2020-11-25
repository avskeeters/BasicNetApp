#pragma once
#include <memory>

class SocketAddress;

class UDPSocket final
{
public:
	enum class BlockingMode{BLOCKING, NON_BLOCKING};
public:
    // Use SocketUtil::CreateUDPSocket factory to create new instance
    ~UDPSocket();
    int Bind(SocketAddress const& toAddress);
    int SendTo(void const* dataToSend, int dataLength, SocketAddress const& toAddress);
    int ReceiveFrom( void* dataBuffer, int maxDataLength, SocketAddress& fromAddress );
    int SetBlockingMode(BlockingMode blockingMode);
private:
    UDPSocket(void* hardwareSpecificSocket); // SOCKET for windows, int for linux?
    friend class SocketUtilities;
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
using UDPSocketPtr = std::shared_ptr<UDPSocket>;