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
	cout << "���� ��� ����(0:����, 1:Ŭ��) >>> ";
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
			cout << "��Ʈ ���� >>> ";
			cin >> Port;

			Server = new SocketServer();
			Server->Initalize();
			if (false == Server->OpenServer(Port))
			{
				cout << "���� ���� ����" << endl;
			}
			break;
		}
		case NET::CLIENT:
		{
			//Client = new SocketClient();






			
			break;
		}
	}

	





	// �Ҹ��� ȣ��
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