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
	mCurrentWindow = GameWindows::Connect;

	mEngine = std::make_unique<Graphics::Engine>(hwnd, width, height, false);
	mMap = std::make_unique<World::Map>("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/Mapas/mapa1.map");

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

	//nk_begin 	Starts a new window; needs to be called every frame for every window (unless hidden) or otherwise the window gets removed 
}

Game::~Game()
{
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
	if (nk_begin(mNuklearContext, "Conectar", nk_rect(50, 50, 400, 200), NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
	{
		static char nameInput[30];
		static char passInput[30];
		static int	textLength[2];

		nk_layout_row_dynamic(mNuklearContext, 0, 2);
		nk_label(mNuklearContext, "Usuario", NK_TEXT_LEFT);
		nk_edit_string(mNuklearContext, NK_EDIT_FIELD, &nameInput[0], &textLength[0], 30, nk_filter_default);
		nk_label(mNuklearContext, "Contrasena", NK_TEXT_LEFT);
		nk_edit_string(mNuklearContext, NK_EDIT_FIELD, &passInput[0], &textLength[1], 30, nk_filter_default);

		nk_layout_row_dynamic(mNuklearContext, 0, 2);
		/*if (*/nk_button_label(mNuklearContext, "Crear Personaje");/*)
			crearpj*/
		/*if (*/nk_button_label(mNuklearContext, "Conectar");/*)
			conectar*/
	}
	nk_end(mNuklearContext);

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