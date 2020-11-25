#include "NetworkingLib/SocketAddress.h"
#include "NetworkingLib/Shared.h"

class SocketAddress::Impl
{
    public:
        sockaddr sockAddr;
};

SocketAddress::SocketAddress() : pImpl{std::make_unique<Impl>()} {}

SocketAddress::SocketAddress(uint32_t ipv4Address, uint16_t port) : SocketAddress()
{
    auto* sockAddrInPtr = GetAsSockAddrInPtr();
    sockAddrInPtr->sin_family = AF_INET; // IPV4
    sockAddrInPtr->sin_addr.S_un.S_addr = htonl(ipv4Address);
    sockAddrInPtr->sin_port = htons(port);
}

// Used when network returns a sockaddr and want to wrap in this typesafe class
SocketAddress::SocketAddress(sockaddr const& sockAddr) : SocketAddress()
{
    memcpy(&pImpl->sockAddr, &sockAddr, sizeof(sockaddr));
}

size_t SocketAddress::Size() const
{
    return sizeof(sockaddr);
}

SocketAddressPtr SocketAddress::CreateIPv4FromString(std::string const& input)
{
    std::string host, service;
    if(auto const colonPos = input.find_last_of(':'); colonPos != std::string::npos)
    {
        host = input.substr(0, colonPos);
        service = input.substr(colonPos + 1);
    }
    else
    {
        host = input;
        service = "0"; // default port
    }
    
    addrinfo hint{};
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;

    addrinfo* result;
    auto const errorCode = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if(errorCode != 0 && result != nullptr) // Something went wrong. Maybe malformed
    {
        freeaddrinfo(result); // automatically walks the linked list
        return nullptr;
    }

    while(!result->ai_addr && result->ai_next) // Search for valid result
    {
        result = result->ai_next;
    }

    if(!result->ai_addr) // Request formed ok but no result found
    {
        freeaddrinfo(result);
        return nullptr;
    }

    SocketAddressPtr out = std::make_shared<SocketAddress>(*result->ai_addr);
    freeaddrinfo(result);
    return out;
}

sockaddr_in const* SocketAddress::GetAsSockAddrInPtr() const
{
    return reinterpret_cast<sockaddr_in const*>(&pImpl->sockAddr);
}

sockaddr_in* SocketAddress::GetAsSockAddrInPtr()
{
    return reinterpret_cast<sockaddr_in*>(&pImpl->sockAddr);
}

sockaddr const* SocketAddress::GetAsSockAddrPtr() const
{
    return &pImpl->sockAddr;
}

sockaddr* SocketAddress::GetAsSockAddrPtr()
{
    return &pImpl->sockAddr;
}
