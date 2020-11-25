#pragma once
#include <cstdint>
#include <memory>
#include <string>

struct sockaddr;
struct sockaddr_in;

class SocketAddress
{
public:
    ~SocketAddress();
    SocketAddress(uint32_t ipv4Address, uint16_t port);
    SocketAddress(sockaddr const& sockAddr);
    [[nodiscard]] size_t Size() const;
public:
    static std::shared_ptr<SocketAddress> CreateIPv4FromString(std::string const& input);
    [[nodiscard]] sockaddr_in const*  GetAsSockAddrInPtr() const;
    [[nodiscard]] sockaddr_in*        GetAsSockAddrInPtr();
    [[nodiscard]] sockaddr const*     GetAsSockAddrPtr() const;
    [[nodiscard]] sockaddr*           GetAsSockAddrPtr();
private:
    SocketAddress();
private:
    friend class UDPSocket;
    friend class TCPSocket;
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
using SocketAddressPtr = std::shared_ptr<SocketAddress>;

