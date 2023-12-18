#include "BinaryBuffer.h"

namespace Utils
{
	template<typename T>
	void BinaryBuffer::WriteData(T value)
	{
		UInt32 size = sizeof(value);

		if (mBufferPosition + size > mBufferLength)
		{
			mBufferLength += (mBufferPosition + size) - mBufferLength;
			mBuffer.resize(mBufferLength);
		}

		*reinterpret_cast<T*>(&mBuffer[mBufferPosition]) = value;
		mBufferPosition += size;
	}

	template <typename T>
	T BinaryBuffer::ReadData()
	{
		UInt32 size = sizeof(T);

		if (mBufferPosition > mBufferLength - size)
		{
			return NULL;
		}

		T* data = reinterpret_cast<T*>(mBuffer.data() + mBufferPosition);
		mBufferPosition += size;

		return *data;
	}

	BinaryBuffer::BinaryBuffer()
	{
		mBuffer.resize(DEFAULT_BUFFER_SIZE);
		mBufferLength = DEFAULT_BUFFER_SIZE;
		mBufferPosition = 0;
	}

	BinaryBuffer::BinaryBuffer(Vector<UInt8> source, UInt32 length) noexcept
	{
		mBuffer = std::move(source);
		mBufferLength = length;
		mBufferPosition = 0;
	}

	UInt8 BinaryBuffer::ReadUInt8()
	{
		return ReadData<UInt8>();
	}

	Int8 BinaryBuffer::ReadInt8()
	{
		return ReadData<Int8>();
	}

	Int16 BinaryBuffer::ReadInt16()
	{
		return ReadData<Int16>();
	}

	Int32 BinaryBuffer::ReadInt32()
	{
		return ReadData<Int32>();
	}

	Float BinaryBuffer::ReadFloat()
	{
		return ReadData<Float>();
	}

	UInt16 BinaryBuffer::ReadUInt16()
	{
		return ReadData<UInt16>();
	}

	UInt32 BinaryBuffer::ReadUInt32()
	{
		return ReadData<UInt32>();
	}

	String BinaryBuffer::ReadString()
	{
		UInt32 size = ReadUInt16();

		if (mBufferPosition > mBufferLength - size)
		{
			return "";
		}

		return String(reinterpret_cast<const char*>(mBuffer.data() + mBufferPosition), size);

		mBufferPosition += size;
	}

	void BinaryBuffer::WriteInt8(Int8 value)
	{
		WriteData<Int8>(value);
	}

	void BinaryBuffer::WriteInt16(Int16 value)
	{
		WriteData<Int16>(value);
	}

	void BinaryBuffer::WriteInt32(Int32 value)
	{
		WriteData<Int32>(value);
	}

	void BinaryBuffer::WriteFloat(Float value)
	{
		WriteData<Float>(value);
	}

	void BinaryBuffer::WriteUInt8(UInt8 value)
	{
		WriteData<UInt8>(value);
	}

	void BinaryBuffer::WriteUInt16(UInt16 value)
	{
		WriteData<UInt16>(value);
	}

	void BinaryBuffer::WriteUInt32(UInt32 value)
	{
		WriteData<UInt32>(value);
	}

	void BinaryBuffer::WriteString(const char* value)
	{
		UInt16 size = strlen(value);

		if (value == 0) return;

		WriteUInt16(size);

		if (mBufferPosition + size > mBufferLength)
		{
			mBufferLength += (mBufferPosition + size) - mBufferLength;
			mBuffer.resize(mBufferLength);
		}

		memcpy(&mBuffer[mBufferPosition], value, size);

		mBufferPosition += size;
	}


	void BinaryBuffer::SkipBytes(UInt32 amount)
	{
		if (mBufferPosition <= mBufferLength - amount)
			mBufferPosition += amount;
	}

	void BinaryBuffer::Flip()
	{
		mBufferLength = mBufferPosition;
		mBufferPosition = 0;
	}

	void BinaryBuffer::Rewind()
	{
		mBufferPosition = 0;
	}

	Bool BinaryBuffer::IsEOF() const
	{
		return mBufferPosition >= mBufferLength;
	}

	Vector<UInt8> BinaryBuffer::GetBuffer() const
	{
		return mBuffer;
	}

	UInt32 BinaryBuffer::GetBufferLength() const
	{
		return mBufferLength;
	}

	UInt32 BinaryBuffer::GetBufferPosition() const
	{
		return mBufferPosition;
	}
}