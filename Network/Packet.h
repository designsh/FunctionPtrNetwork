#pragma once
#include "Serializer.h"

class Packet : public std::enable_shared_from_this<Packet>
{
public:
	template<typename ConvertType>
	std::shared_ptr<ConvertType> DynamicConvert()
	{
		return std::dynamic_pointer_cast<ConvertType>(shared_from_this());
	}

	template<typename EnumType>
	EnumType GetPacketID()
	{
		return static_cast<EnumType>(PacketID_);
	}

public:
	void SerializePacket(Serializer& _Ser)
	{
		Packet::Serialize(_Ser);
		Serialize(_Ser);
		SerializeEnd(_Ser);
	}

	void DeSerializePacket(Serializer& _Ser)
	{
		Packet::DeSerialize(_Ser);
		DeSerialize(_Ser);
		DeSerializeEnd(_Ser);
	}

protected:
	virtual void Serialize(Serializer& _Ser) = 0
	{
		_Ser << PacketID_;
		_Ser << PakcetSize_;
	}

	void SerializeEnd(Serializer& _Ser)
	{
		PakcetSize_ = _Ser.GetOffSet();
		if (PakcetSize_ == 0)
		{
			assert(false);
		}

		unsigned char* Ptr = _Ser.GetDataPtr();
		memcpy_s(&Ptr[4], sizeof(int), &PakcetSize_, sizeof(int));
	}

	virtual void DeSerialize(Serializer& _Ser) = 0
	{
		_Ser >> PacketID_;
		_Ser >> PakcetSize_;
	}

	void DeSerializeEnd(Serializer& _Ser)
	{
	}

private:

public:
	Packet()
		: PacketID_(0), PakcetSize_(0)
	{
	}

	~Packet()
	{
	}

protected:
	//Packet(const Packet& _other) = delete;
	//Packet(Packet&& _other) = delete;

private:
	//Packet& operator=(const Packet& _rhs) = delete;
	//Packet& operator=(Packet&& _rhs) = delete;

public:
	int PacketID_;
	int PakcetSize_;

protected:
private:
};

