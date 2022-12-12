#pragma once
#include "Network.h"

class SocketClient : public Network
{
public:
protected:
private:

public:
	SocketClient();
	~SocketClient();

protected:
	SocketClient(const SocketClient& _other) = delete;
	SocketClient(SocketClient&& _other) = delete;

private:
	SocketClient& operator=(const SocketClient& _rhs) = delete;
	SocketClient& operator=(SocketClient&& _rhs) = delete;

public:
protected:
private:

};

