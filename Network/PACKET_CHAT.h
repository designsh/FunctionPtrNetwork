#pragma once
#include "Packet.h"

struct Packet_Chat : public Packet
{
	std::string ChatText;

	void Serialize(Serializer& _Ser) override
	{
		Packet::Serialize(_Ser);

		_Ser << ChatText;
	}

	virtual void DeSerialize(Serializer& _Ser) override
	{
		Packet::DeSerialize(_Ser);

		_Ser >> ChatText;
	}
};

