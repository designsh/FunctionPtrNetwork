#include "mainHeader.h"

// Network
#include "SocketClient.h"
#include "SocketServer.h"

// Packet
#include "Packet_Chat.h"

NET CurNet = NET::MAX;

static SocketClient* Client = nullptr;
static SocketServer* Server = nullptr;

void AcceptCallbackFunction(SOCKET _Socket)
{
	int AcceptID = -1;
	if (nullptr != Server)
	{
		AcceptID = Server->GetAcceptID();
	}

	if (-1 != AcceptID)
	{
		std::string Msg;
		Msg += std::to_string(AcceptID);
		Msg += "번 클라이언트 연결 성공!!!";
		cout << Msg << endl;
	}
}

void ChatFunction(std::shared_ptr<Packet> _Packet)
{
	std::shared_ptr<Packet_Chat> Ptr = std::dynamic_pointer_cast<Packet_Chat>(_Packet);

	cout << Ptr->ChatText << endl;
}

std::shared_ptr<Packet> ContentsPacketCheck(int _PacketType, int _PacketSize, const char* _Data)
{
	// 패킷 타입 변경이 필요함!!!!

	Serializer Ser = Serializer(_Data, _PacketSize);

	std::shared_ptr<Packet> NewPacket = nullptr;

	PACKET_TYPE Type = static_cast<PACKET_TYPE>(_PacketType);

	switch (Type)
	{
	case PACKET_TYPE::LOGIN:
		
		break;
	case PACKET_TYPE::CHAT:
		NewPacket = std::make_shared<Packet_Chat>();
		break;
	default:
		break;
	}

	NewPacket->DeSerializePacket(Ser);

	return NewPacket;
}

int main()
{
	cout << "<<< TCP/IP Echo Socket Program Start >>>" << endl;

	int NetNum = -1;
	cout << "소켓 기능 선택(0:서버, 1:클라) >>> ";
	cin >> NetNum;
	if (0 > NetNum || NetNum >= static_cast<int>(NET::MAX))
	{
		cout << "Error!!!!" << endl;
		return 0;
	}

	// Select Network & Add Packet
	CurNet = static_cast<NET>(NetNum);
	switch (CurNet)
	{
		case NET::SERVER:
		{
			int Port = 0;
			cout << "포트 설정 >>> ";
			cin >> Port;

			Server = new SocketServer();
			Server->SetAcceptCallBack(AcceptCallbackFunction);

			// 서버 소켓 패킷 등록
			Server->Dispatcher_.SetPacketConvertCallBack(ContentsPacketCheck);
			Server->Dispatcher_.AddHandler(PACKET_TYPE::CHAT, ChatFunction);


			// 서버 오픈
			Server->Initalize();
			if (false == Server->OpenServer(Port))
			{
				cout << "서버 오픈 실패" << endl;
			}
			break;
		}
		case NET::CLIENT:
		{
			std::string IP;
			int Port = 0;

			cout << "아이피 설정 >>> ";
			cin >> IP;
			cout << "포트 설정 >>> ";
			cin >> Port;

			Client = new SocketClient();

			// 클라 소켓 패킷 등록
			Client->Dispatcher_.SetPacketConvertCallBack(ContentsPacketCheck);
			Client->Dispatcher_.AddHandler(PACKET_TYPE::CHAT, ChatFunction);


			// 서버연결
			Client->Initalize();
			if (false == Client->ConnectClient(IP, Port))
			{
				cout << "서버 연결 실패" << endl;
			}
			break;
		}
	}

	int ExitFlag = 0;
	while (true)
	{
		cout << "종료 하시겠습니까(1:종료) >> ";
		cin >> ExitFlag;

		if (1 == ExitFlag)
		{
			cout << "종료!!!" << endl;
			break;
		}

		std::string Msg;
		cout << "메세지입력: >> ";
		cin >> Msg;

		Packet_Chat Packet;
		Packet.PacketID_ = static_cast<int>(PACKET_TYPE::CHAT);
		Packet.ChatText = Msg;
		if (nullptr != Server)
		{
			Server->SendPacket(Packet);
		}
		else if (nullptr != Client)
		{
			Client->SendPacket(Packet);
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