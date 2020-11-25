#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

// Standard integral types
#include <cstdint>

#include "NetworkingLib/SocketUtilities.h"
#include "NetworkingLib/TCPSocket.h"
#include "NetworkingLib/UDPSocket.h"
#include "NetworkingLib/SocketAddress.h"