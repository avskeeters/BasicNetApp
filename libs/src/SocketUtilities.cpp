#include "NetworkingLib/Shared.h"
#include <strsafe.h>

bool SocketUtilities::InitWinSockets()
{
	WSADATA wsaData;
    if(auto const startupError = WSAStartup(MAKEWORD(2, 2), &wsaData); startupError != NO_ERROR)
    {
        ShowLastError("SocketUtilities::InitWinSockets");
        return false;
    }
    return true;
}

void SocketUtilities::Cleanup()
{
    WSACleanup();
}

int SocketUtilities::GetLastErrorCode()
{
    return WSAGetLastError();
}

DWORD SocketUtilities::ShowLastError( LPCSTR lpszFunction, bool const inWindow )
{
    // Not mine. From:
    // https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code?redirectedfrom=MSDN

    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastErrorCode();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);

    if (inWindow)
    {
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
    }

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

    return dw;
}

void SocketUtilities::ShowLastErrorAndExit( LPCSTR lpszFunction, bool const inWindow )
{
    auto const errorCode = ShowLastError(lpszFunction, inWindow);
    ExitProcess(errorCode);
}

UDPSocketPtr SocketUtilities::CreateUDPSocket( SocketAddressFamily socketAddrFamily )
{
    auto const addressFamilyInt = static_cast<int>(socketAddrFamily);
    SOCKET newSocket = socket(addressFamilyInt, SOCK_DGRAM, IPPROTO_UDP);
	if(newSocket != INVALID_SOCKET)
	{
        return UDPSocketPtr{ new UDPSocket(&newSocket) };
	}

    ShowLastError("SocketUtilities::CreateUDPSocket");
    return nullptr;
}

TCPSocketPtr SocketUtilities::CreateTCPSocket( SocketAddressFamily socketAddrFamily )
{
    auto const addressFamilyInt = static_cast<int>(socketAddrFamily);
    SOCKET newSocket = socket(addressFamilyInt, SOCK_STREAM, IPPROTO_TCP);
	if(newSocket != INVALID_SOCKET)
	{
        return TCPSocketPtr{ new TCPSocket(&newSocket) };
	}
    ShowLastError("SocketUtilities::CreateTCPSocket");
    return nullptr;
}

int SocketUtilities::Select( std::vector<TCPSocketPtr> const* checkForReadable,
	std::vector<TCPSocketPtr>* outputReadable, std::vector<TCPSocketPtr> const* checkForWritable,
	std::vector<TCPSocketPtr>* outputWritable, std::vector<TCPSocketPtr> const* checkForExceptions,
	std::vector<TCPSocketPtr>* outputExceptions )
{
    fd_set readSet;
    fd_set writeSet;
    fd_set exceptSet;

    auto* readSetPtr = VectorToFDSet(readSet, checkForReadable);
    auto* writeSetPtr = VectorToFDSet(writeSet, checkForWritable);
    auto* exceptSetPtr = VectorToFDSet(exceptSet, checkForExceptions);

    auto const total = select(1, readSetPtr, writeSetPtr, exceptSetPtr, nullptr);
	if(total > 0)
	{
        FDSetToVector(outputReadable, checkForReadable, readSet);
        FDSetToVector(outputWritable, checkForWritable, writeSet);
        FDSetToVector(outputExceptions, checkForExceptions, exceptSet);
	}

    return total;
}

fd_set* SocketUtilities::VectorToFDSet( fd_set& setToFill, std::vector<TCPSocketPtr> const* sockets )
{
	if(sockets)
	{
        FD_ZERO(&setToFill);
		for(auto const& _socket : *sockets)
		{
            FD_SET(*static_cast<SOCKET const*>(_socket->GetRawSocketPtr()), &setToFill);
		}
        return &setToFill;
	}
    return nullptr;
}

void SocketUtilities::FDSetToVector( std::vector<TCPSocketPtr>* vectorToFill,
	std::vector<TCPSocketPtr> const* vectorToPullFrom, fd_set const& setToFill )
{
	if(vectorToFill && vectorToPullFrom)
	{
        vectorToFill->clear();
		for(auto const& _socket : *vectorToPullFrom)
		{
			// See if socket in set
			if(FD_ISSET(*static_cast<SOCKET const*>(_socket->GetRawSocketPtr()), &setToFill))
			{
                vectorToFill->push_back(_socket);
			}
		}
	}
}



