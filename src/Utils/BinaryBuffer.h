#pragma once

#include <vector>
#include <Defines.h>

namespace Utils
{
	class BinaryBuffer
	{
	private:
		static constexpr Int32 DEFAULT_BUFFER_SIZE = 1024;

		Vector<UInt8> 	mBuffer;
		UInt32			mBufferLength;
		UInt32			mBufferPosition;

		template<typename T>
		void		WriteData(T value);

		template<typename T>
		T			ReadData();

	public:

		BinaryBuffer();
		BinaryBuffer(Vector<UInt8> source, UInt32 length) noexcept;

		Bool		IsEOF() const;

		Int8		ReadInt8();
		Int16		ReadInt16();
		Int32		ReadInt32();
		Float		ReadFloat();

		UInt8		ReadUInt8();
		UInt16		ReadUInt16();
		UInt32		ReadUInt32();
		String		ReadString();

		void		WriteInt8(Int8 value);
		void		WriteInt16(Int16 value);
		void		WriteInt32(Int32 value);
		void		WriteFloat(Float value);

		void		WriteUInt8(UInt8 value);
		void		WriteUInt16(UInt16 value);
		void		WriteUInt32(UInt32 value);
		void		WriteString(const char* value);

		void		SkipBytes(UInt32 amount);
		void		Flip();
		void		Rewind();

		Vector<UInt8>	GetBuffer() const;
		UInt32			GetBufferLength() const;
		UInt32			GetBufferPosition() const;
	};
}