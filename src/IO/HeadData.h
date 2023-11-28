#pragma once

#include <vector>
#include <Defines.h>
#include <filesystem>
#include <fstream>

#include <Graphics/Grh.h>
#include <Utils/BinaryReader.h>

namespace IO
{
	class HeadData
	{
	public:
		Graphics::Grh	Head[4] = {};

		static Vector<HeadData> LoadHeadData(const Vector<IO::GrhData>& grhList, Bool isHelmet = false)
		{
			Vector<HeadData> headData;

			auto fileSize = std::filesystem::file_size(isHelmet ? "D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Cascos.ind" : "D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Cabezas.ind");
		

			if (fileSize <= 0)
				return {};

			auto buffer = Vector<UInt8>(fileSize);

			std::ifstream stream(isHelmet ? "D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Cascos.ind" : "D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Cabezas.ind", std::ios::binary | std::ios::ate);


			stream.seekg(0, std::ios::beg);
			stream.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
			stream.close();

			auto reader = Utils::BinaryReader(std::move(buffer), fileSize);

			reader.SkipBytes(263);

			Int16 headCount = reader.ReadInt16();
			headData.resize(headCount + 1);

			for (Int32 i = 1, j = headCount; i <= j; ++i)
			{
				headData[i].Head[0] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				headData[i].Head[1] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				headData[i].Head[2] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
				headData[i].Head[3] = Graphics::Grh(grhList, reader.ReadInt16(), 0);
			}

			return headData;
		}
	};
}