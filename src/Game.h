#pragma once
#include <Windows.h>

#include <Defines.h>
#include <Graphics/Engine.h>
#include <World/Map.h>


#define MAX_NK_VERTEX_BUFFER 512 * 1024
#define MAX_NK_INDEX_BUFFER 128 * 1024

struct nk_context;
//struct nk_colorf;

#include <UI/ConnectWindow.h>

class Game
{
private:

	UPtr<Graphics::Engine>	mEngine = nullptr;
	UPtr<World::Map>		mMap	= nullptr;
	UPtr<Network::Protocol> mProtocol = nullptr;

	nk_context*				mNuklearContext = nullptr;
	//nk_colorf				mNuklearBackgroundColor;// = { 1.0f, 1.0f, 1.0f, 1.0f };

	enum class GameWindows
	{
		Connect,
		CharCreation,
		Main
	};

	Vector<UPtr<UI::INuklearWindow>> mGameWindows;

public:
	
	Game(HWND hwnd, int width, int height);
	~Game();

	void OnDraw();
	void OnUpdate();
	int OnInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UIInputCaptureBegin();
	void UIInputCaptureEnd();

	void FlushProtocol();
};
