#pragma once

#include <vector>
#include <Defines.h>

namespace Utils
{
	class BinaryReader
	{
	private:

		static constexpr Int32 DEFAULT_BUFFER_SIZE = 1024;

		Vector<UInt8> 	mBuffer;
		UInt32			mBufferLength;
		UInt32			mBufferPosition;


		template<typename T>
		T* ReadData();
	public:
		BinaryReader();
		BinaryReader(Vector<UInt8> &&source, UInt32 length) noexcept;

		Int8		ReadInt8();
		Int16		ReadInt16();
		Int32		ReadInt32();
		Float		ReadFloat();

		UInt8		ReadUInt8();
		UInt16		ReadUInt16();
		UInt32		ReadUInt32();
		
		void SkipBytes(UInt32 amount);

		Bool IsEOF();
	};
}