#pragma once

#include <vector>
#include <Defines.h>
#include <filesystem>
#include <fstream>

#include <Utils/BinaryBuffer.h>

namespace IO
{
	class GrhData
	{
	public:
		UInt32				SX = 0;
		UInt32				SY = 0;
		UInt32				FileNum = 0;
		UInt32				PixelWidth = 0;
		UInt32				PixelHeight = 0;
		Float				TileWidth = 0.0f;
		Float				TileHeight = 0.0f;
		UInt32				NumFrames = 0;

		Vector<UInt32>		Frames;

		Float				Speed = 0.0f;

		static Vector<GrhData> LoadGrhData()
		{
			Vector<GrhData> grhList;

			auto fileSize = std::filesystem::file_size("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Graficos3.ind");

			if (fileSize <= 0)
				return {};

			auto buffer = Vector<UInt8>(fileSize);

			std::ifstream stream("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/INIT/Graficos3.ind", std::ios::binary | std::ios::ate);
			stream.seekg(0, std::ios::beg);
			stream.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
			stream.close();

			auto reader = Utils::BinaryBuffer(std::move(buffer), fileSize);

			Int32 version = reader.ReadInt32();
			Int32 grhCount = reader.ReadInt32();

			grhList.resize(grhCount + 1);

			while (!reader.IsEOF())
			{
				Int32 grh = reader.ReadInt32();

				if (grh == 0) continue;

				Int32 numFrames = reader.ReadInt16();

				grhList[grh].Frames.resize(numFrames);

				if (numFrames > 1)
				{
					for (Int32 i = 0; i < numFrames; ++i)
					{
						Int32 frame = reader.ReadInt32();

						if (frame <= 0 || frame > grhCount) return {};

						grhList[grh].Frames[i] = frame;
					}

					grhList[grh].Speed = reader.ReadFloat();

					if (grhList[grh].Speed <= 0) return {};

					grhList[grh].PixelHeight = grhList[grhList[grh].Frames[0]].PixelHeight;
					if (grhList[grh].PixelHeight <= 0) return {};

					grhList[grh].PixelWidth = grhList[grhList[grh].Frames[0]].PixelWidth;
					if (grhList[grh].PixelWidth <= 0) return {};

					grhList[grh].TileHeight = grhList[grhList[grh].Frames[0]].TileHeight;
					if (grhList[grh].TileHeight <= 0) return {};

					grhList[grh].TileWidth = grhList[grhList[grh].Frames[0]].TileWidth;
					if (grhList[grh].TileWidth <= 0) return {};
				}
				else
				{
					grhList[grh].FileNum = reader.ReadInt32();
					if (grhList[grh].FileNum <= 0) return {};

					grhList[grh].SX = reader.ReadInt16();
					if (grhList[grh].SX < 0) return {};

					grhList[grh].SY = reader.ReadInt16();
					if (grhList[grh].SY < 0) return {};

					grhList[grh].PixelWidth = reader.ReadInt16();
					if (grhList[grh].PixelWidth <= 0) return {};

					grhList[grh].PixelHeight = reader.ReadInt16();
					if (grhList[grh].PixelHeight <= 0) return {};

					grhList[grh].TileWidth = (Float)grhList[grh].PixelWidth / 32;
					grhList[grh].TileHeight = (Float)grhList[grh].PixelHeight / 32;

					grhList[grh].Frames[0] = grh;
				}
			}

			return grhList;
		}
	};
}