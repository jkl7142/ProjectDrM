#pragma once

#include <WinSock2.h>
#include <windows.h>
#include <mswsock.h>

#include <string>

enum class SocketType {
	Tcp,
	Udp
};

class Socket {
public:

	Socket();
	Socket(SOCKET socket);
	Socket(SocketType socketType);
	~Socket();

	// Socket API
	void Init();
	void Bind(const Endpoint& endpoint);
	void Connect(const Endpoint& endpoint);
	int Send(const char* data, int length);
	void Listen();

	int Accept(Socket& acceptedSocket, std::string& error);
	bool AcceptOverlapped(Socket& acceptCandidateSocket, std::string& error);
	int Receive();
	int ReceiveOverlapped();
	void Close();

	int UpdateAcceptContext(Socket& listenSocket);
	Endpoint GetPeerAddr();
	void SetNonblocking();

	// socket handle
	SOCKET m_socket;

protected:
	static const int MaxReceiveLength = 8192;

	// AcceptEx 함수 포인터
	LPFN_ACCEPTEX AcceptEx = NULL;

	// Overlapped I/O나 IOCP 가능 변수
	bool m_isReadOverlapped = false;

	// Overlapped Recv or Accept 시 사용되는 overlapped 객체
	WSAOVERLAPPED m_readOverlappedStruct;

	// 수신 버퍼
	char m_receiveBuffer[MaxReceiveLength];

	// overlapped 수신 플래그
	DWORD m_readFlags = 0;
};

std::string GetLastErrorAsString();

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")