// Client.AO.cpp: define el punto de entrada de la aplicación.
//
#include <Windows.h>
#include <Game.h>
#include <Network/Connection.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

UPtr<Game> game = nullptr;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR cmdArgs, _In_ INT cmdShow)
{
	HWND hWnd{ 0 };
	WNDCLASSEX wc{ 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"ArgentumClient";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 1366, 768 };

	AdjustWindowRectEx(&wr, WS_POPUP, false, WS_EX_APPWINDOW);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"ArgentumClient", L"ArgentumClient", WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, (wr.right - wr.left), (wr.bottom - wr.top), 0, NULL, hInstance, NULL);

	//test
	auto con = std::make_unique<Network::Connection>();
	//test

	game = std::make_unique<Game>(hWnd, 1366, 768);

	ShowWindow(hWnd, cmdShow);

	MSG msg = { 0 };
	
	Bool gameRunning = true;

	while (gameRunning)
	{
		game->UIInputCaptureBegin();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				gameRunning = false;
		}
		game->UIInputCaptureEnd();

		game->OnDraw();

	}

	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return msg.wParam;
}

void OnKeyDown(WPARAM wParam)
{
	WORD vkCode = LOWORD(wParam);

	switch (vkCode)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_CHAR:
		OnKeyDown(wParam);
		break;
		/*case WM_SIZE:
			if (swap_chain)
			{
				int width = LOWORD(lparam);
				int height = HIWORD(lparam);
				set_swap_chain_size(width, height);
				nk_d3d12_resize(width, height);
			}
			break;
		}
		*/
	}

	if (game->OnInput(hWnd, message, wParam, lParam))
		return 0;

	return DefWindowProcW(hWnd, message, wParam, lParam);
}
