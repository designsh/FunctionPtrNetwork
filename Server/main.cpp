#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment (lib, "ws2_32")

using namespace std;

#define PACKET_SIZE 1024

WSADATA wsa;
SOCKET skt, *Client_skt;
SOCKADDR_IN* Client;
int* Client_size, ClientMax;

void ReceiveFunction(SOCKET& s, int _ClientNum)
{
	char Buf[PACKET_SIZE] = {};

	while (true)
	{
		ZeroMemory(Buf, PACKET_SIZE);
		if (-1 == recv(s, Buf, PACKET_SIZE, 0))
		{
			break;
		}

		cout << "\nClient #" << _ClientNum << " << " << Buf << "\n보낼 데이터를 입력 >> ";
	}

	return;
}

void AcceptFunction()
{
	char client_num[10] = {};
	for (int i = 0; i < ClientMax; ++i)
	{
		Client_size[i] = sizeof(Client[i]);
		Client_skt[i] = accept(skt, (SOCKADDR*)&Client[i], &Client_size[i]);
		
		if (Client_skt[i] == INVALID_SOCKET)
		{
			cout << "Accept Eror" << endl;
			closesocket(Client_skt[i]);
			closesocket(skt);
			WSACleanup();
			return;
		}

		cout << "Client #" << i << " Joined!" << "\n보낼 데이터를 입력 >>";
		ZeroMemory(client_num, sizeof(client_num));
		sprintf_s(client_num, "%d", i);
		send(Client_skt[i], client_num, static_cast<int>(strlen(client_num)), 0);
		thread(ReceiveFunction, std::ref(Client_skt[i]), i).detach();
	}
}

void SocketOpen(int _Port)
{
	// WinSock Initalize
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		cout << "WSA Error" << endl;
		return;
	}

	// Create Socket
	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET)
	{
		cout << "Socket Error" << endl;
		closesocket(skt);
		WSACleanup();
		return;
	}

	// Socket Bind
	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_Port);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(skt, (SOCKADDR*)&addr, sizeof(addr)))
	{
		cout << "Bind Error" << endl;
		closesocket(skt);
		WSACleanup();
		return;
	}

	// 연결되는 클라이언트 대기
	if (listen(skt, SOMAXCONN))
	{
		cout << "Listen Error" << endl;
		closesocket(skt);
		WSACleanup();
		return;
	}

	// Accept Thread
	thread(AcceptFunction).detach();

	// Server Socket Send Process
	char ExitMsg[PACKET_SIZE] = {};
	char msg[PACKET_SIZE] = {};
	char sendnum[PACKET_SIZE] = {};

	while (true)
	{
		cout << "종료하시겠습니까?(Y:종료) >> ";
		cin >> ExitMsg;

		if (!strcmp(ExitMsg, "Y"))
		{
			break;
		}
		else
		{
			cout << "보낼 데이터를 입력 >>";
			cin >> msg;

			cout << "대상 클라이언트를 입력(all:모두) >> ";
			cin >> sendnum;

			if (!strcmp(sendnum, "all"))
			{
				for (int i = 0; i < ClientMax; ++i)
				{
					send(Client_skt[i], msg, static_cast<int>(strlen(msg)), 0);
				}
			}
			else
			{
				send(Client_skt[atoi(sendnum)], msg, static_cast<int>(strlen(msg)), 0);
			}
		}

		// Server Exit
		for (int i = 0; i < ClientMax; ++i)
		{
			closesocket(Client_skt[i]);
		}
		closesocket(skt);
		WSACleanup();
		return;
	}
}

int main()
{
	cout << "Function Pointer Networkd(SERVER) Start" << endl;

	int PORT = 9999;
	cout << "포트 설정 >> ";
	cin >> PORT;
	cout << "클라이언트 최대수용 수 설정 >> ";
	cin >> ClientMax;

	Client_skt = new SOCKET[ClientMax];
	Client = new SOCKADDR_IN[ClientMax];
	Client_size = new int[ClientMax];

	ZeroMemory(Client_skt, sizeof(Client_skt) - 1);
	ZeroMemory(Client, sizeof(Client) - 1);
	//ZeroMemory(Client_size, sizeof(Client_size) - 1);

	// Socket Opent
	SocketOpen(PORT);

	// Memory Release
	delete[] Client_skt, Client, Client_size;

	// Server Program End
	cout << "Function Pointer Networkd(SERVER) End" << endl;
	return 0;
}