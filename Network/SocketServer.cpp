#include "SocketServer.h"

bool SocketServer::bOpen = false;

void SocketServer::Initalize()
{
	WSAData wsaData;
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (SOCKET_ERROR == errorCode)
	{
		cout << "Winsock 초기화에 실패하였습니다!!!" << endl;
		return;
	}
}

bool SocketServer::OpenServer(int _Port)
{
	Port_ = _Port;
	if (0 != Socket_)
	{
		cout << "이미 서버가 열려있는 상태입니다!!!" << endl;
		return false;
	}

	Socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == Socket_)
	{
		cout << "소켓 생성 실패" << endl;
		return false;
	}

	SOCKADDR_IN addr = { 0, };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port_);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr))
	{
		cout << "주소 설정 실패" << endl;
		return false;
	}

	if (SOCKET_ERROR == ::bind(Socket_, (const sockaddr*)&addr, sizeof(SOCKADDR_IN)))
	{
		cout << "IP 주소 및 포트 바인딩 실패" << endl;
		return false;
	}

	if (SOCKET_ERROR == listen(Socket_, BackLog_))
	{
		cout << "서버 리슨상태 전환 실패" << endl;
		return false;
	}

	// Server Open Flag On
	bOpen = true;

	// Client Accept Process Function
	AcceptThread_ = new std::thread(std::bind(&SocketServer::AcceptFunction, this));

	return true;
}

void SocketServer::CloseServer()
{
	Mutex_.lock();

	if (nullptr != AcceptThread_)
	{
		closesocket(Socket_);
		Socket_ = 0;
		AcceptThread_->join();
		delete AcceptThread_;
		AcceptThread_ = nullptr;
	}

	for (SOCKET s : ClientList_)
	{
		closesocket(s);
	}
	ClientList_.clear();

	Mutex_.unlock();

	// Client Recevie Thread Join
	auto Begin = ClientReceiveThreadList_.begin();
	auto End = ClientReceiveThreadList_.end();
	while (Begin != End)
	{
		if (Begin->second.joinable())
		{
			Begin->second.join();
		}
		Begin++;
	}

	//if (nullptr != packetHandler_)
	//{
	//	delete packetHandler_;
	//	packetHandler_ = nullptr;
	//}




	// Server Open Flag Off
	bOpen = false;
}

void SocketServer::Send()
{

}

void SocketServer::AcceptFunction()
{
	int size = sizeof(SOCKADDR_IN);
	SOCKADDR_IN addr = { 0, };
	while (true)
	{
		SOCKET NewClient = accept(Socket_, (sockaddr*)&addr, &size);
		if (INVALID_SOCKET == NewClient)
		{
			return;
		}

		Mutex_.lock();

		ClientList_.push_back(NewClient);

		std::thread newReceiveThread(std::bind(&SocketServer::ReceiveFunction, this, NewClient));
		ClientReceiveThreadList_.insert(std::pair<SOCKET, std::thread>(NewClient, std::move(newReceiveThread)));

		Mutex_.unlock();
	}
}

void SocketServer::ReceiveFunction(SOCKET _Socket)
{
	char Packet[PACKET_SIZE] = { 0 };
	while (true)
	{
		int retValue = recv(_Socket, Packet, sizeof(Packet), 0);
		if (0 < retValue)
		{
			// 수신한 패킷이 존재
			

			cout << "수신 했다!!!!" << endl;



		}
		else if(SOCKET_ERROR == retValue)
		{
			// 패킷 수신 실패
			Mutex_.lock();
			if (bOpen == false)
			{
				Mutex_.unlock();
				return;
			}

			for (size_t i = 0; i < ClientList_.size(); i++)
			{
				if (_Socket == ClientList_[i])
				{
					ClientList_[i] = ClientList_.back();
					ClientList_.pop_back();
					break;
				}
			}

			auto findThreadIter = ClientReceiveThreadList_.find(_Socket);
			if (findThreadIter != ClientReceiveThreadList_.end())
			{
				findThreadIter->second.detach();
				ClientReceiveThreadList_.erase(_Socket);
			}

			Mutex_.unlock();
		}

		ZeroMemory(Packet, PACKET_SIZE);
	}
}

SocketServer::SocketServer()
	: Port_(8080)
	, BackLog_(512)
	, Socket_()
	, AcceptThread_(nullptr)
{
}

SocketServer::~SocketServer()
{
	CloseServer();
}
