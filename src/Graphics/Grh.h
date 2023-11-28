#pragma once

#include <vector>
#include <Defines.h>
#include <IO/GrhData.h>

namespace Graphics
{
	class Grh
	{
	public:

		UInt8	Started = 0;
		UInt32	GrhIndex = 0;
		UInt32	Loops = 0;
		Float	FrameCounter = 0.0f;
		Float	Speed = 0.0f;

		Grh() = default;

		Grh(UInt32 grhIndex, Float frameCounter, Float speed, UInt8 started, UInt32 loops)
			:GrhIndex{ grhIndex },
			FrameCounter{ frameCounter },
			Speed{ speed },
			Started{ started },
			Loops{ loops }
		{

		}

		Grh(const Vector<IO::GrhData>& grhList, UInt32 grhIndex, UInt8 started = 2)
		{
			GrhIndex = grhIndex;

			if (started == 2)
			{
				Started = grhList[grhIndex].NumFrames > 1 ? 1 : 0;

			}
			else
			{
				Started = grhList[grhIndex].NumFrames == 1 ? 0 : started;
			}

			Loops = (Started ? -1 : 0);
			FrameCounter = 1.0f; //0?
			Speed = grhList[grhIndex].Speed;
		}
	};
}