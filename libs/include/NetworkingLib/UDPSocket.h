#pragma once
#include <memory>

class SocketAddress;

class UDPSocket final
{
public:
    // Use SocketUtil::CreateUDPSocket factory to create new instance
    ~UDPSocket();
    int Bind(SocketAddress const& toAddress);
    int SendTo(void const* dataToSend, int dataLength, SocketAddress const& toAddress);
    int ReceiveFrom( void* dataBuffer, int maxDataLength, SocketAddress& fromAddress );
private:
    UDPSocket(void* hardwareSpecificSocket); // SOCKET for windows, int for linux?
private:
    friend class SocketUtilities;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
using UDPSocketPtr = std::shared_ptr<UDPSocket>;