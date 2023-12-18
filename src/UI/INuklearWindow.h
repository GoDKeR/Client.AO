#pragma once

#include <Defines.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include <nuklear.h>
#include <UI/D3D11Nuklear.h>
#include <Network/Protocol.h>

struct nk_context;

namespace UI
{
	class INuklearWindow
	{
	private:

	protected:
		Int32 mWindowID = -1;
		Bool mVisible = true; //Window is visible by default
		String mWindowTitle = "";
		
		Network::Protocol* pProtocol;

	public:		
		virtual void DrawWindow(nk_context* nuklearContext, Network::Protocol* protocolPtr) = 0;

		Bool IsVisible() const
		{
			return mVisible;
		}

		void SetVisible(Bool value)
		{
			mVisible = value;
		}
	};
}