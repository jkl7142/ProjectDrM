#include "stdafx.h"

#include <rpc.h>

#include "Socket.h"
#include "Endpoint.h"
#include "Exception.h"

// 소켓을 생성하지 않음
Socket::Socket() {
	static_assert(INVALID_SOCKET == -1, "");

	m_socket = -1;

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

// 외부 소켓 핸들
Socket::Socket(SOCKET socket) {
	Init();

	m_socket = socket;

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

// 소켓 생성
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
void Socket::Bind(const Endpoint& endpoint) {
	if (bind(m_socket, (sockaddr*)&endpoint.m_ipv4Endpoint, sizeof(endpoint.m_ipv4Endpoint)) < 0) {
		std::stringstream ss;
		ss << "bind failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}
void Socket::Connect(const Endpoint& endpoint) {
	if (connect(m_socket, (sockaddr*)&endpoint.m_ipv4Endpoint, sizeof(endpoint.m_ipv4Endpoint)) < 0) {
		std::stringstream ss;
		ss << "connect failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}

int Socket::Send(const char* data, int length) {
	return ::send(m_socket, data, length, 0);
}

void Socket::Listen() {
	if (listen(m_socket, 5000) < 0) {
		std::stringstream ss;
		ss << "listen failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}

int Socket::Accept(Socket& acceptedSocket, std::string& error) {
	acceptedSocket.m_socket = accept(m_socket, NULL, 0);
	if (acceptedSocket.m_socket < 0) {
		error = GetLastErrorAsString();
		return -1;
	}
	else
		return 0;
}


// 소켓풀에 존재하는 소켓이 CandidateSocket으로 들어가며 accept되고 나면 TCP 연결 객체로 바뀐다.
bool Socket::AcceptOverlapped(Socket& acceptCandidateSocket, std::string& error) {
	if (AcceptEx == NULL) {
		DWORD bytes;
		UUID uuid = UUID(WSAID_ACCEPTEX);
		// AcceptEx는 직접 호출하는 것이 아니고,
		// 함수 포인터를 먼저 가져온 다음 호출할 수 있다.
		WSAIoctl(
			m_socket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&uuid,
			sizeof(UUID),
			&AcceptEx,
			sizeof(AcceptEx),
			&bytes,
			NULL,
			NULL);

		if (AcceptEx == NULL)
		{
			throw Exception("Getting AcceptEx ptr failed.");
		}
	}


	char lpOutputBuffer[200];
	DWORD lpdwBytesReceived = 0;

	bool ret = AcceptEx(
					m_socket,
					acceptCandidateSocket.m_socket,
					&lpOutputBuffer,	// 소켓의 로컬, 원격 주소와 첫번째 바이트 스트림을 수신할 버퍼
					0,
					50,
					50,
					&lpdwBytesReceived,	// 실제로 수신한 바이트 수
					&m_readOverlappedStruct
				) == TRUE;

	return ret;
}

int Socket::Receive() {
	return static_cast<int>(recv(m_socket, m_receiveBuffer, MaxReceiveLength, 0));
}

// 수신되는 데이터는 m_receiveBuffer에 비동기로 채워짐
int Socket::ReceiveOverlapped() {
	WSABUF lpBuffer;
	lpBuffer.buf = m_receiveBuffer;
	lpBuffer.len = MaxReceiveLength;

	// overlapped I/O가 진행중이면, 해당 플래그가 on됨
	m_readFlags = 0;

	return WSARecv(m_socket, &lpBuffer, 1, NULL, &m_readFlags, &m_readOverlappedStruct, NULL);
}

void Socket::Close() {
	closesocket(m_socket);
}

int Socket::UpdateAcceptContext(Socket& listenSocket) {
	char lpOutputBuffer[100];
	sockaddr_in localSockAddr;	// 로컬 sockaddr 포인터
	sockaddr_in remoteSockAddr;	// 원격 sockaddr 포인터
	INT localLen, remoteLen;	// sockaddr 길이

	GetAcceptExSockaddrs(
		lpOutputBuffer,
		0,
		50,
		50,
		(sockaddr**)&localSockAddr,
		&localLen,
		(sockaddr**)&remoteSockAddr,
		&remoteLen);

	return setsockopt(m_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
					(char*)&listenSocket.m_socket, sizeof(listenSocket.m_socket));
}

Endpoint Socket::GetPeerAddr() {
	Endpoint ret;
	socklen_t retLength = sizeof(ret.m_ipv4Endpoint);

	if (::getpeername(m_socket, (sockaddr*)&ret.m_ipv4Endpoint, &retLength) < 0) {
		std::stringstream ss;
		ss << "getPeerAddr failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}

	if (retLength > sizeof(ret.m_ipv4Endpoint)) {
		std::stringstream ss;
		ss << "getPeerAddr buffer overrun: " << retLength;
		throw Exception(ss.str().c_str());
	}

	return ret;
}

void Socket::SetNonblocking() {
	u_long val = 1;

	if (ioctlsocket(m_socket, FIONBIO, &val) < 0) {
		std::stringstream ss;
		ss << "bind failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}

std::string GetLastErrorAsString() {
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string();

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	LocalFree(messageBuffer);
	
	return message;
}