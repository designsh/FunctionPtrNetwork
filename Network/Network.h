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

#include "Dispatcher.h"

#define PACKET_SIZE 1024

#pragma comment (lib, "ws2_32")

using namespace std;

class Network
{
public:
	void SendPacket(Packet& Data)
	{
		Serializer Ser;
		Data.SerializePacket(Ser);

		Send(reinterpret_cast<char*>(Ser.GetDataPtr()), Ser.GetOffSet());
	}

protected:
	virtual void Send(const char* Data, size_t _Size) = 0;

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
	Dispatcher Dispatcher_;

protected:
private:
};
