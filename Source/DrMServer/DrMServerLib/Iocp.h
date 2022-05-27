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

	// GetQueuedCompletionStatusEx 1�� ȣ�� �� �ִ��� ������ �� �ִ� �۾��� ����
	static const int MaxEventCnt = 1000;

private:
	HANDLE m_hIocp;
	int m_threadCnt;	// IOCP ���� ��, ���� �߰� �� ��� ���Ǵ� ��
};

class IocpEvents {
public:
	// GetQueuedCompletionStatusEx()�� ť���� ������ �̺�Ʈ
	OVERLAPPED_ENTRY m_events[Iocp::MaxEventCnt];
	int m_eventCnt;
};