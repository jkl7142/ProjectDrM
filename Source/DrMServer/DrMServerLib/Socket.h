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
	Socket(int socket);
	Socket(SocketType socketType);
	~Socket();

	// Socket API
	void Init();
	void Bind();
	void Connect();
	void Listen();
	int Accept(Socket& acceptedSocket, std::string& error);
	int Send(const char* data, int length);
	int Receive();
	void Close();

	// Socket Overlapped I/O
	bool AcceptOverlapped(Socket& acceptCandidateSocket, std::string& error);
	int UpdateAcceptContext(Socket& listenSocket);
	int ReceiveOverlapped();

	// GetPeerAddr();
	// void SetNonbloking();

protected:
	static const int MaxReceiveLength = 8192;

	// socket handle
	int m_socket;

	// AcceptEx �Լ� ������
	LPFN_ACCEPTEX AcceptEx = NULL;

	// Overlapped I/O�� IOCP ���� ����
	bool m_isReadOverlapped = false;

	// Overlapped Recv or Accept �� ���Ǵ� overlapped ��ü
	WSAOVERLAPPED m_readOverlappedStruct;

	// ���� ����
	char m_receiveBuffer[MaxReceiveLength];

	// overlapped ���� �÷���
	DWORD m_readFlags = 0;
};

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")