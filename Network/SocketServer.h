#pragma once
#include "Network.h"

class SocketServer : public Network
{
public:
	static bool bOpen;

public:
	void Initalize();
	bool OpenServer(int _Port);
	void CloseServer();

protected:
	void Send() override;

private:
	void AcceptFunction();
	void ReceiveFunction(SOCKET _Socket);

public:
	SocketServer();
	~SocketServer();

protected:
	SocketServer(const SocketServer& _other) = delete;
	SocketServer(SocketServer&& _other) = delete;

private:
	SocketServer& operator=(const SocketServer& _rhs) = delete;
	SocketServer& operator=(SocketServer&& _rhs) = delete;

public:
protected:
private:
	std::mutex Mutex_;
	int Port_;
	int BackLog_;

private:
	SOCKET Socket_;
	std::thread* AcceptThread_;

private:
	std::vector<SOCKET> ClientList_;
	std::map<SOCKET, std::thread> ClientReceiveThreadList_;

private: // Packet Value

};

