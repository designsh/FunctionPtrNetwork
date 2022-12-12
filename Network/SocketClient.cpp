#include "SocketClient.h"

bool SocketClient::bConnect = false;

void SocketClient::Initalize()
{
	WSAData wsaData;
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (SOCKET_ERROR == errorCode)
	{
		cout << "Winsock 초기화에 실패하였습니다!!!" << endl;
		return;
	}
}

bool SocketClient::ConnectClient(const std::string& _IP, int _Port)
{
	IP_ = _IP;
	Port_ = _Port;

	if (0 != Socket_)
	{
		cout << "이미 클라이언트가 열결되어있는 상태입니다!!!" << endl;
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
	if (SOCKET_ERROR == inet_pton(AF_INET, IP_.c_str(), &addr.sin_addr))
	{
		cout << "주소 설정 실패" << endl;
		return false;
	}

	int retValue = connect(Socket_, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == retValue)
	{
		cout << "서버 연결 실패" << endl;
		return false;
	}

	bConnect = true;
	if (ReceiveThread_ == nullptr)
	{
		ReceiveThread_ = new std::thread(std::bind(&SocketClient::ReceiveFunction, this, Socket_));
	}

	return true;
}

void SocketClient::DisConnectClient()
{
	if (0 == Socket_)
	{
		cout << "연결된 서버가 없음!!!" << endl;
		return;
	}
	closesocket(Socket_);

	if (nullptr != ReceiveThread_)
	{
		ReceiveThread_->join();
		delete ReceiveThread_;
		ReceiveThread_ = nullptr;
	}
}

void SocketClient::Send(const char* Data, size_t _Size)
{
	// 서버와 연결된 소켓으로 송신
	send(Socket_, Data, static_cast<int>(_Size), 0);
}

void SocketClient::ReceiveFunction(SOCKET _Socket)
{
	char Packet[PACKET_SIZE] = { 0 };

	while (true)
	{
		int retValue = recv(Socket_, Packet, sizeof(Packet), 0);
		if (0 < retValue)
		{
			// 패킷 수신처리
			Dispatcher_.PacketCheck(Packet);
		}

		if (SOCKET_ERROR == retValue)
		{
			// 패킷 수신 실패
			cout << "서버 연결 종료" << endl;
			return;
		}

		ZeroMemory(Packet, PACKET_SIZE);
	}
}

SocketClient::SocketClient()
	: IP_("127.0.0.1")
	, Port_(8080)
	, Socket_()
	, ReceiveThread_(nullptr)
{
}

SocketClient::~SocketClient()
{
	DisConnectClient();
}
