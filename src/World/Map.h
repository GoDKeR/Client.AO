#pragma once

#include <vector>
#include <fstream>
#include <filesystem>
#include <string_view>

#include <Graphics/Engine.h>
#include <Utils/BinaryBuffer.h>

namespace World
{
	class Map
	{
	private:

		struct WorldPos
		{
			Int32 Map	= 0;
			Int32 X		= 0;
			Int32 Y		= 0;
		};

		struct Obj
		{
			Int32 ObjIndex	= 0;
			Int32 Amount	= 0;
		};

		

		static constexpr Int32 MAP_WIDTH = 100;

		std::string_view	MapFile;
	public:
		struct Tile
		{
			Graphics::Grh Graphic[4];
			Graphics::Grh ObjGrh;

			Int32 CharIndex = 0;
			Int32 NpcIndex = 0;
			Obj ObjInfo = { 0, 0 };
			WorldPos Exit = { 0, 0, 0 };
			Bool Blocked = false;

			Int32 Trigger = 0;

		};

		Tile* MapData = new Tile[MAP_WIDTH * MAP_WIDTH];

		Map() = default;
		Map(std::string_view fileName);
		~Map();

		Tile& GetTile(Int32 x, Int32 y);
	};
}