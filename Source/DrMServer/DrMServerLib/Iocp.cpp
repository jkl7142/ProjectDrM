#include "stdafx.h"
#include "Iocp.h"
#include "Socket.h"

Iocp::Iocp(int threadCnt) {
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, threadCnt);
	m_threadCnt = threadCnt;
}

Iocp::~Iocp() {
	CloseHandle(m_hIocp);
}

// IOCP에 socket 추가
void Iocp::Add(Socket& socket, void* userPtr) {
	if (!CreateIoCompletionPort((HANDLE)socket.m_socket, m_hIocp, (ULONG_PTR)userPtr, m_threadCnt))
		// throw Exception("IOCP add failed!"); TODO : Exception 클래스
}

void Iocp::Wait(IocpEvents& output, int timeoutMs) {
	if (!GetQueuedCompletionStatusEx(m_hIocp, output.m_events, MaxEventCnt,
									(ULONG*)&output.m_eventCnt, timeoutMs, FALSE)) {
		output.m_eventCnt = 0;
	}
}