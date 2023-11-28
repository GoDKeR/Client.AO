#pragma once
#include <Windows.h>

#include <Defines.h>
#include <Graphics/Engine.h>
#include <World/Map.h>


#define MAX_NK_VERTEX_BUFFER 512 * 1024
#define MAX_NK_INDEX_BUFFER 128 * 1024

struct nk_context;
//struct nk_colorf;

class Game
{
private:

	enum GameWindows
	{
		Connect,
		CharCreation,
		Main
	};

	GameWindows				mCurrentWindow = GameWindows::Connect;

	UPtr<Graphics::Engine>	mEngine = nullptr;
	UPtr<World::Map>		mMap	= nullptr;

	nk_context*				mNuklearContext = nullptr;
	//nk_colorf				mNuklearBackgroundColor;// = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	
	Game(HWND hwnd, int width, int height);
	~Game();

	void OnDraw();
	void OnUpdate();
	int OnInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void UIInputCaptureBegin();
	void UIInputCaptureEnd();
};
