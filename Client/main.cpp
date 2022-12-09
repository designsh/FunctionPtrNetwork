#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <thread>

#pragma comment (lib, "ws2_32")

using namespace std;

#define PACKET_SIZE 1024

void ReceiveFunction(SOCKET& s)
{
	char Buffer[PACKET_SIZE] = {};

	while (true)
	{
		ZeroMemory(Buffer, PACKET_SIZE);
		int Result = recv(s, Buffer, static_cast<int>(strlen(Buffer)), 0);
		if (0 < Result)
		{
			cout << "\n[Server] >> " << Buffer << "\n보낼 데이터 입력 >> ";
		}
		else
		{
			int a = 0;
		}

		// 서버 종료 감지
		if (WSAGetLastError())
		{
			break;
		}
	}

	return;
}

void SocketOpen(char _IP[], int _Port)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		cout << "WSA Error" << endl;
		WSACleanup();
		return;
	}

	SOCKET skt;
	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET)
	{
		cout << "Socket Error" << endl;
		closesocket(skt);
		WSACleanup();
		return;
	}

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_Port);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (SOCKET_ERROR == inet_pton(AF_INET, _IP, &addr.sin_addr))
	{
		cout << "Socket Address Bind Error" << endl;
		closesocket(skt);
		WSACleanup();
		return;
	}

	while (connect(skt, (SOCKADDR*)&addr, sizeof(addr)));

	char Buf[PACKET_SIZE] = {};
	recv(skt, Buf, PACKET_SIZE, 0);
	int MyNumber = atoi(Buf);
	cout << "[" << MyNumber << "]" << addr.sin_addr.S_un.S_addr << "::" << _Port << endl;

	thread(ReceiveFunction, std::ref(skt)).detach();

	// 서버가 연결중이라면
	while (!WSAGetLastError())
	{
		cout << "보낼 데이터를 입력 >> ";
		cin >> Buf;

		send(skt, Buf, static_cast<int>(strlen(Buf)), 0);
	}

	closesocket(skt);
	WSACleanup();
}

int main()
{
	cout << "Function Pointer Networkd(CLIENT) Start" << endl;

	char IP[100] = {};
	int Port = 8080;

	cout << "아이피주소 설정 >> ";
	cin >> IP;
	cout << "포트 설정 >> ";
	cin >> Port;

	SocketOpen(IP, Port);

	cout << "Function Pointer Networkd(CLIENT) End" << endl;
	return 0;
}