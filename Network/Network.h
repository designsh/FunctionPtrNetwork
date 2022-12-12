#pragma once
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>

#include <thread>
#include <mutex>

#include <vector>
#include <map>

#include <string>
#include <functional>

#define PACKET_SIZE 1024

#pragma comment (lib, "ws2_32")

using namespace std;

class Network
{
public:
protected:
	virtual void Send() = 0;

private:

public:
	Network();
	~Network();

protected:
	Network(const Network& _other) = delete;
	Network(Network&& _other) = delete;

private:
	Network& operator=(const Network& _rhs) = delete;
	Network& operator=(Network&& _rhs) = delete;

public:
protected:
private:

};

