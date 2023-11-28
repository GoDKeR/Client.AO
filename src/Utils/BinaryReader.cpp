#include "BinaryReader.h"

namespace Utils
{
	template <typename T>
	T* BinaryReader::ReadData()
	{
		UInt32 size = sizeof(T);

		if (mBufferPosition <= mBufferLength - size)
		{

			T* data = reinterpret_cast<T*>(mBuffer.data() + mBufferPosition);
			mBufferPosition += size;

			return data;
		}

		return nullptr;
	}

	BinaryReader::BinaryReader()
	{
		mBuffer.resize(DEFAULT_BUFFER_SIZE);
		mBufferLength = DEFAULT_BUFFER_SIZE;
		mBufferPosition = 0;
	}

	BinaryReader::BinaryReader(Vector<UInt8> &&source, UInt32 length) noexcept
	{
		mBuffer			= source;
		mBufferLength	= length;
		mBufferPosition = 0;
	}

	UInt8 BinaryReader::ReadUInt8()
	{
		return *ReadData<UInt8>();
	}

	Int8 BinaryReader::ReadInt8()
	{
		return *ReadData<Int8>();
	}

	Int16 BinaryReader::ReadInt16()
	{
		return *ReadData<Int16>();
	}

	Int32 BinaryReader::ReadInt32()
	{
		return *ReadData<Int32>();
	}

	Float BinaryReader::ReadFloat()
	{
		return *ReadData<Float>();
	}

	UInt16 BinaryReader::ReadUInt16()
	{
		return *ReadData<UInt16>();
	}

	UInt32 BinaryReader::ReadUInt32()
	{
		return *ReadData<UInt32>();
	}

	void BinaryReader::SkipBytes(UInt32 amount)
	{
		if (mBufferPosition <= mBufferLength - amount)
			mBufferPosition += amount;
	}

	Bool BinaryReader::IsEOF()
	{
		return mBufferPosition >= mBufferLength;
	}
}