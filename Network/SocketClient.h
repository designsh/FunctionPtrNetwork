#pragma once
#include "Network.h"

class SocketClient : public Network
{
public:
	static bool bConnect;

public:
	void Initalize();
	bool ConnectClient(const std::string& _IP, int _Port);
	void DisConnectClient();

protected:
	void Send() override;

private:
	void ReceiveFunction(SOCKET _Socket);

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
	std::mutex Mutex_;
	std::string IP_;
	int Port_;
	
private:
	SOCKET Socket_;

private:
	std::thread* ReceiveThread_;

private: // Packet Value

};

