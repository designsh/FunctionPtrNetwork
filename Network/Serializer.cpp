#include "Serializer.h"

void Serializer::Read(void* Data, unsigned int _Size)
{
	memcpy_s(Data, _Size, &Data_[Offset_], _Size);
	Offset_ += _Size;
}

void Serializer::Write(const void* Data, unsigned int _Size)
{
	if (Offset_ + _Size >= Data_.size())
	{
		Data_.resize(Data_.size() * 2);
	}

	memcpy_s(&Data_[Offset_], _Size, Data, _Size);
	Offset_ += _Size;
}

void Serializer::operator<<(const int _Value)
{
	Write(&_Value, sizeof(int));
}

void Serializer::operator<<(const float _Value)
{
	Write(&_Value, sizeof(float));
}

void Serializer::operator<<(const std::string& _Value)
{
	operator<<(static_cast<int>(_Value.size()));
	Write(reinterpret_cast<const void*>(&_Value[0]), static_cast<unsigned int>(_Value.size()));
}

void Serializer::operator<<(const unsigned int _Value)
{
	Write(&_Value, sizeof(unsigned int));
}

void Serializer::operator<<(const uint64_t& _Value)
{
	Write(&_Value, sizeof(uint64_t));
}

void Serializer::operator<<(const float4& _Value)
{
	Write(&_Value, sizeof(float4));
}


void Serializer::operator>>(std::string& _Value)
{
	int Size;
	operator>>(Size);
	_Value.resize(Size);
	Read(&_Value[0], Size);
}

void Serializer::operator>>(int& _Value)
{
	Read(&_Value, sizeof(int));
}

void Serializer::operator>>(unsigned int& _Value)
{
	Read(&_Value, sizeof(unsigned int));
}

void Serializer::operator>>(float& _Value)
{
	Read(&_Value, sizeof(float));
}

void Serializer::operator>>(float4& _Value)
{
	Read(&_Value, sizeof(float4));
}

void Serializer::operator>>(uint64_t& _Value)
{
	Read(&_Value, sizeof(uint64_t));
}

Serializer::Serializer()
{
	Offset_ = 0;
	Data_.resize(1024);
}

Serializer::Serializer(const std::vector<unsigned char>& _Data)
	: Offset_(0)
	, Data_(_Data)
{

}
Serializer::Serializer(const char* _Data, unsigned int _Size)
	: Offset_(0)
{
	Data_.resize(_Size);
	memcpy_s(&Data_[0], _Size, _Data, _Size);
}
