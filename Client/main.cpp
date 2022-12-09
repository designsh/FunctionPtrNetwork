#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment (lib, "ws2_32")

using namespace std;

#define PACKET_SIZE 1024

void ReceiveFunction(SOCKET& s)
{

}

void SocketOpen(char _IP[], int _Port)
{

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