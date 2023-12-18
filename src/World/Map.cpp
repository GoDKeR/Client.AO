#include "Map.h"

namespace World
{
	Map::~Map()
	{
		delete[] MapData;
	}

	Map::Map(std::string_view fileName)
	{
		auto fileSize = std::filesystem::file_size(fileName);

		if (fileSize <= 0)
			return;

		auto buffer = Vector<UInt8>(fileSize);

		std::ifstream stream(fileName.data(), std::ios::binary | std::ios::ate);
		stream.seekg(0, std::ios::beg);
		stream.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
		stream.close();
		
		auto reader = Utils::BinaryBuffer(std::move(buffer), fileSize);
		
		reader.SkipBytes(273);
		
		for (auto y = 0; y < 100; ++y)
		{
			for (auto x = 0; x < 100; ++x)
			{
				auto flag = reader.ReadUInt8();

				//MapData[x][y].Blocked = flag & 0x01;
				MapData[y * MAP_WIDTH + x].Blocked = flag & 0x01;

				auto grh = reader.ReadInt16();
				MapData[y * MAP_WIDTH + x].Graphic[0].GrhIndex = grh;
				MapData[y * MAP_WIDTH + x].Graphic[0] = Graphics::Grh(Graphics::Engine::GrhList, grh);
				
				if (flag & 0x02)
				{
					grh = reader.ReadInt16();
					MapData[y * MAP_WIDTH + x].Graphic[1].GrhIndex = grh;
					MapData[y * MAP_WIDTH + x].Graphic[1] = Graphics::Grh(Graphics::Engine::GrhList, grh);
				}
				else
				{
					MapData[y * MAP_WIDTH + x].Graphic[1].GrhIndex = 0;
				}

				if (flag & 0x04)
				{
					grh = reader.ReadInt16();
					MapData[y * MAP_WIDTH + x].Graphic[2].GrhIndex = grh;
					MapData[y * MAP_WIDTH + x].Graphic[2] = Graphics::Grh(Graphics::Engine::GrhList, grh);
				}
				else
				{
					MapData[y * MAP_WIDTH + x].Graphic[2].GrhIndex = 0;
				}

				if (flag & 0x08)
				{
					grh = reader.ReadInt16();
					MapData[y * MAP_WIDTH + x].Graphic[3].GrhIndex = grh;
					MapData[y * MAP_WIDTH + x].Graphic[3] = Graphics::Grh(Graphics::Engine::GrhList, grh);
				}
				else
				{
					MapData[y * MAP_WIDTH + x].Graphic[3].GrhIndex = 0;
				}

				if (flag & 0x10)
				{
					MapData[y * MAP_WIDTH + x].Trigger = reader.ReadInt16();
				}
				else
				{
					MapData[y * MAP_WIDTH + x].Trigger = 0;
				}

				if (MapData[y * MAP_WIDTH + x].CharIndex > 0)
				{
					//todo: erase
				}

				MapData[y * MAP_WIDTH + x].ObjGrh.GrhIndex = 0;
			}
		}
	}

	Map::Tile& Map::GetTile(Int32 x, Int32 y)
	{
		return MapData[y * MAP_WIDTH + x];
	}
}