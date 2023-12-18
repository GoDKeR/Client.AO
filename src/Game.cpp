#include "Game.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION

#include <nuklear.h>
#include <UI/D3D11Nuklear.h>

Game::Game(HWND hwnd, int width, int height)
{

	mEngine = std::make_unique<Graphics::Engine>(hwnd, width, height, false);
	mMap = std::make_unique<World::Map>("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/Mapas/mapa1.map");
	mProtocol = std::make_unique<Network::Protocol>();

	mGameWindows.push_back(std::make_unique<UI::ConnectWindow>(static_cast<Int32>(GameWindows::Connect), "Conectar", true));

	//UI INIT
	//mNuklearContext = std::make_unique<nk_context>(nk_d3d11_init(mEngine->GetDevice().Get(), width, height, MAX_NK_VERTEX_BUFFER, MAX_NK_INDEX_BUFFER));
	mNuklearContext = nk_d3d11_init(mEngine->GetDevice().Get(), width, height, MAX_NK_VERTEX_BUFFER, MAX_NK_INDEX_BUFFER);
	//mNuklearBackgroundColor = { 0.1f, 0.18f, 0.24f, 1.0f };

	{
		nk_font_atlas* atlas;
		nk_d3d11_font_stash_begin(&atlas);
		/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../extra_font/DroidSans.ttf", 14, 0);*/
		/*struct nk_font *robot = nk_font_atlas_add_from_file(atlas, "../../extra_font/Roboto-Regular.ttf", 14, 0);*/
		/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
		/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyClean.ttf", 12, 0);*/
		/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyTiny.ttf", 10, 0);*/
		/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../extra_font/Cousine-Regular.ttf", 13, 0);*/
		nk_d3d11_font_stash_end();
	}
}

Game::~Game()
{
	mProtocol->Disconnect();
	nk_d3d11_shutdown();
}

void Game::OnDraw()
{
	const Float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	mEngine->Clear(color, 1.0f, 0);

	mEngine->SetBuffers();
	mEngine->ResetStates();

	mEngine->Render(mMap.get());

	mEngine->Flush();

	mEngine->DepthStateDisabled();
	//UI thingy things goes here

	for (auto& win : mGameWindows)
	{
		win->DrawWindow(mNuklearContext, mProtocol.get());
	}

	nk_d3d11_render(mEngine->GetDeviceContext().Get(), NK_ANTI_ALIASING_ON);
	mEngine->DepthStateEnabled();

	mEngine->Present(); //todo: send vsync to present?
}

void Game::OnUpdate()
{
	//all the timing stuff should be here?
}

int Game::OnInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return nk_d3d11_handle_event(hWnd, message, wParam, lParam);
}

void Game::UIInputCaptureBegin()
{
	nk_input_begin(mNuklearContext);
}

void Game::UIInputCaptureEnd()
{
	nk_input_end(mNuklearContext);
}

void Game::FlushProtocol()
{
	mProtocol->SendData();
}