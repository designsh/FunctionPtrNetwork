#include "Dispatcher.h"

void Dispatcher::PacketCheck(const char* Data)
{
	int PacketType = 0;
	int PacketSize = 0;

	// 패킷 헤더 분석
	memcpy_s(&PacketType, sizeof(int), Data, sizeof(int));
	memcpy_s(&PacketSize, sizeof(int), Data + 4, sizeof(int));

	// Callback Function 등록
	std::shared_ptr<Packet> Packet = PacketReturnCallBack_(PacketType, PacketSize, Data);
	PacketProcessMap_[Packet->PacketID_](Packet);
}

void Dispatcher::AddHandler(int _Type, std::function<void(std::shared_ptr<Packet>)> _Packet)
{
	PacketProcessMap_[_Type] = _Packet;
}

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
}
