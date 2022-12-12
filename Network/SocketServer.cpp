#include "SocketServer.h"

bool SocketServer::bOpen = false;

void SocketServer::Initalize()
{
	WSAData wsaData;
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (SOCKET_ERROR == errorCode)
	{
		cout << "Winsock �ʱ�ȭ�� �����Ͽ����ϴ�!!!" << endl;
		return;
	}
}

bool SocketServer::OpenServer(int _Port)
{
	Port_ = _Port;
	if (0 != Socket_)
	{
		cout << "�̹� ������ �����ִ� �����Դϴ�!!!" << endl;
		return false;
	}

	Socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == Socket_)
	{
		cout << "���� ���� ����" << endl;
		return false;
	}

	SOCKADDR_IN addr = { 0, };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port_);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr))
	{
		cout << "�ּ� ���� ����" << endl;
		return false;
	}

	if (SOCKET_ERROR == ::bind(Socket_, (const sockaddr*)&addr, sizeof(SOCKADDR_IN)))
	{
		cout << "IP �ּ� �� ��Ʈ ���ε� ����" << endl;
		return false;
	}

	if (SOCKET_ERROR == listen(Socket_, BackLog_))
	{
		cout << "���� �������� ��ȯ ����" << endl;
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
			// ������ ��Ŷ�� ����
			

			cout << "���� �ߴ�!!!!" << endl;



		}
		else if(SOCKET_ERROR == retValue)
		{
			// ��Ŷ ���� ����
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
