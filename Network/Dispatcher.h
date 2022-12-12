#pragma once
#include <functional>
#include <map>
#include "Packet.h"

class Dispatcher
{
public:
	void PacketCheck(const char* Data);

public:
	void SetPacketConvertCallBack(std::function <std::shared_ptr<Packet>(int _PacketType, int _PacketSize, const char* Data)> _Packet)
	{
		PacketReturnCallBack_ = _Packet;
	}

public:
	template<typename EnumType>
	void AddHandler(EnumType _Type, std::function<void(std::shared_ptr<Packet>)> _Packet)
	{
		AddHandler(static_cast<int>(_Type), _Packet);
	}

public:
	void AddHandler(int _Type, std::function<void(std::shared_ptr<Packet>)> _Packet);

protected:
private:

public:
	Dispatcher();
	~Dispatcher();

protected:
	Dispatcher(const Dispatcher& _Other) = delete;
	Dispatcher(Dispatcher&& _Other) noexcept = delete;

private:
	Dispatcher& operator=(const Dispatcher& _Other) = delete;
	Dispatcher& operator=(Dispatcher&& _Other) noexcept = delete;

public:
protected:
private:
	std::map<int, std::function<void(std::shared_ptr<Packet> _Packet)>> PacketProcessMap_;
	std::function <std::shared_ptr<Packet>(int _PacketType, int _PacketSize, const char* Data)> PacketReturnCallBack_;
};

