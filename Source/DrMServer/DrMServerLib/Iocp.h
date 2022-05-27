#pragma once

class Socket;
class IocpEvents;

// I/O Completion Port
class Iocp {
public:
	Iocp(int threadCnt);
	~Iocp();

	void Add(Socket& socket, void* userPtr);
	void Wait(IocpEvents& output, int timeoutMs);

	// GetQueuedCompletionStatusEx 1번 호출 시 최대한 가져올 수 있는 작업의 개수
	static const int MaxEventCnt = 1000;

private:
	HANDLE m_hIocp;
	int m_threadCnt;	// IOCP 생성 시, 소켓 추가 시 계속 사용되는 값
};

class IocpEvents {
public:
	// GetQueuedCompletionStatusEx()로 큐에서 가져온 이벤트
	OVERLAPPED_ENTRY m_events[Iocp::MaxEventCnt];
	int m_eventCnt;
};