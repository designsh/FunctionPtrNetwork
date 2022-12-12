#include "mainHeader.h"
#include "SocketClient.h"
#include "SocketServer.h"

NET CurNet = NET::MAX;

int main()
{
	cout << "<<< TCP/IP Echo Socket Program Start >>>" << endl;
	SocketClient* Client = nullptr;
	SocketServer* Server = nullptr;

	int NetNum = -1;
	cout << "소켓 기능 선택(0:서버, 1:클라) >>> ";
	cin >> NetNum;
	if (0 > NetNum || NetNum >= static_cast<int>(NET::MAX))
	{
		cout << "Error!!!!" << endl;
		return 0;
	}

	CurNet = static_cast<NET>(NetNum);
	switch (CurNet)
	{
		case NET::SERVER:
		{
			int Port = 0;
			cout << "포트 설정 >>> ";
			cin >> Port;

			Server = new SocketServer();
			Server->Initalize();
			if (false == Server->OpenServer(Port))
			{
				cout << "서버 오픈 실패" << endl;
			}
			break;
		}
		case NET::CLIENT:
		{
			//Client = new SocketClient();






			
			break;
		}
	}

	





	// 소멸자 호출
	if (nullptr != Server)
	{
		delete Server;
	}

	if (nullptr != Client)
	{
		delete Client;
	}

	return 0;
}