#pragma once

#include <WS2tcpip.h>

class Endpoint {
public:
	Endpoint();
	Endpoint(const char* address, int port);
	~Endpoint();

	sockaddr_in m_ipv4Endpoint;

	std::string ToString();
};