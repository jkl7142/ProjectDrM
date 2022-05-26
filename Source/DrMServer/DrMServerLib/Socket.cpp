#include "stdafx.h"

#include <rpc.h>

#include "Socket.h"

// 家南阑 积己窍瘤 臼澜
Socket::Socket() {
	static_assert(INVALID_SOCKET == -1, "");

	m_socket = -1;

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

// 寇何 家南 勤甸
Socket::Socket(int socket) {
	Init();

	m_socket = socket;

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

// 家南 积己
Socket::Socket(SocketType socketType) {
	Init();

	if (socketType == SocketType::Tcp) {
		m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	}

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

Socket::~Socket() {
	Close();
}

void Socket::Init() {
	WSADATA w;
	WSAStartup(MAKEWORD(2, 2), &w);
}

// Socket API
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