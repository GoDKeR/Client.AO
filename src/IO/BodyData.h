#pragma once

#include <vector>
#include <Defines.h>
#include <filesystem>
#include <fstream>

#include <Graphics/Grh.h>
#include <Utils/BinaryReader.h>

namespace IO
{
	class BodyData
	{
	public:
		Vector2F		HeadOffset	= {};
		Graphics::Grh	Walk[4]		= {};

		static Vector<BodyData> LoadBodyData(const Vector<IO::GrhData>& grhList)
		{
			Vector<BodyData> bodyData;

			auto fileSize = std::filesystem::file_size("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Personajes.ind");

			if (fileSize <= 0)
				return {};

			auto buffer = Vector<UInt8>(fileSize);

			std::ifstream stream("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Personajes.ind", std::ios::binary | std::ios::ate);
			stream.seekg(0, std::ios::beg);
			stream.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
			stream.close();

			auto reader = Utils::BinaryReader(std::move(buffer), fileSize);

			reader.SkipBytes(263);

			Int16 bodyCount = reader.ReadInt16();
			bodyData.resize(bodyCount + 1);

			for (Int32 i = 1, j = bodyCount; i <= j; ++i)
			{
				bodyData[i].Walk[0] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				bodyData[i].Walk[1] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				bodyData[i].Walk[2] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				bodyData[i].Walk[3] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				
				bodyData[i].HeadOffset.x = reader.ReadInt16();
				bodyData[i].HeadOffset.y = reader.ReadInt16();
			}

			return bodyData;
		}
	};
}