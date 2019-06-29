// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "Header.h"
#include "Draw/d3d9_draw.h"

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
RECT				rc = {0, 0};
ULONGLONG			timescale = GetTickCount64();
bool				keys[MAX_PATH] = {}, gameOver{ false };
GameSprite			*player;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd), Render(HWND hWnd);
int rand(int min, int max);
void Move(), GameScripts(), CleanupDeviceD3D(), Restart();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct Snake {
	Vector2D pos[MAX_PATH];
	int length{ 3 };
	int side{ 0 };
	int tail{ 0 };
} snake;

struct Food {
	bool active{ false };
	Vector2D pos{-1, -1};
	ULONGLONG last = GetTickCount64();
	GameSprite* image = {};
} food;

struct Pool {
	int width = 17, 
		height = 15;
} pool;

// Main code
int main()
{
	// Create application window
	WNDCLASSEXW wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Snake TEST", NULL };
	RegisterClassExW(&wc);
	HWND hwnd = CreateWindowW(wc.lpszClassName, L"Snake", WS_OVERLAPPEDWINDOW, 100, 100, pool.width * 25 + 60, pool.height * 25 + 83, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	// Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	GetClientRect(hwnd, &rc);

	snake.pos[0] = { 4, 8 };
	snake.pos[1] = { 3, 8 };
	snake.pos[2] = { 2, 8 };

	food.image = new GameSprite();
	food.image->Initialize(g_pd3dDevice, L"images/food.png", 25, 25);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		GameScripts();

		// Renderings
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(0, 0, 0, 255);
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			Render(hwnd);
			g_pd3dDevice->EndScene();
		}

		g_pd3dDevice->Present(0, 0, 0, 0);

	}

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	return true;
}

int rand(int min, int max)
{
	int random = (rand() % (max + 1)) + min;

	return random;
}

void Move()
{
	if ((GetTickCount64() - timescale) >= 100 && snake.side)
	{
		Vector2D prev{ snake.pos[0].x, snake.pos[0].y };
		Vector2D prev2{ snake.pos[0].x, snake.pos[0].y };

		switch (snake.side)
		{
		case 1:
		{
			snake.pos[0].y -= 1;
			snake.tail = 3;
		}
		break;
		case 2:
		{
			snake.pos[0].x -= 1;
			snake.tail = 4;
		}
		break;
		case 3:
		{
			snake.pos[0].y += 1;
			snake.tail = 1;
		}
		break;
		case 4:
		{
			snake.pos[0].x += 1;
			snake.tail = 2;
		}
		break;
		}

		for (int i = 1; i < snake.length; i++)
		{
			prev = prev2;
			prev2 = snake.pos[i];
			if (snake.pos[i].x < prev.x)
			{
				snake.pos[i].x += 1;
			}
			else if (snake.pos[i].x > prev.x)
			{
				snake.pos[i].x -= 1;
			}
			else if (snake.pos[i].y < prev.y)
			{
				snake.pos[i].y += 1;
			}
			else if (snake.pos[i].y > prev.y)
			{
				snake.pos[i].y -= 1;
			}
		}

		timescale = GetTickCount64();
	}
	return;
}

void GameScripts()
{
	if (keys['W'] && snake.side != 1 && snake.tail != 1)
		snake.side = 1;

	if (keys['A'] && snake.side != 2 && snake.tail != 2)
		snake.side = 2;

	if (keys['S'] && snake.side != 3 && snake.tail != 3)
		snake.side = 3;

	if (keys['D'] && snake.side != 4 && snake.tail != 4)
		snake.side = 4;

	keys['W'] = false;
	keys['A'] = false;
	keys['S'] = false;
	keys['D'] = false;

	Vector2D lasttail = { 0, 0 };

	if (!food.active)
	{
		bool insnake{ true };
		Vector2D pos = { 0, 0 };
		while (insnake)
		{
			insnake = false;
			pos = { (float)rand(0, 17), (float)rand(0, 15) };
			for (int i = 0; i < snake.length; i++)
			{
				if (pos == snake.pos[i])
					insnake = true;
			}
		}

		food.pos = pos;
		food.active = true;
	}
	else if (food.active)
	{
		lasttail = snake.pos[snake.length - 1];
	}

	Move();

	if (food.active && food.pos == snake.pos[0])
	{
		snake.length += 1;
		snake.pos[snake.length - 1] = lasttail;
		food.active = false;
		food.pos = { 0, 0 };
		food.last = GetTickCount64();
	}

	for (int i = 0; i < snake.length; i++)
	{
		for (int k = 0; k < snake.length; k++)
		{
			if (i != k && snake.pos[i] == snake.pos[k])
			{
				Restart();
				return;
			}
		}
	}

	if (snake.pos[0].x <= -1 || snake.pos[0].y <= -1 || snake.pos[0].x >= 18 || snake.pos[0].y >= 16)
		Restart();

	return;
}

bool Render(HWND hWnd)
{
	D3D9_Draw::Box(g_pd3dDevice, {8, 8}, (25 * pool.width) + 25 + 3, (25 * pool.height) + 25 + 3, 3, D3DCOLOR_RGBA(255, 255, 255, 100), false);

	for (int i = 0; i < snake.length; i++)
	{
		D3D9_Draw::BoxFilled(g_pd3dDevice, { (snake.pos[i].x * 25) + 10, (snake.pos[i].y * 25) + 10 }, 25, 25, D3DCOLOR_RGBA(0, 255, 0, 100), false);
	}

	if (food.active && food.image && food.image->IsInitialized())
		food.image->Draw((food.pos.x * 25) + 10, (food.pos.y * 25) + 10, 0);

	return true;
}

void Restart()
{
	snake.pos[0] = { 4, 8 };
	snake.pos[1] = { 3, 8 };
	snake.pos[2] = { 2, 8 };

	snake.length = 3;
	snake.side = 0;

	return;
}

void CleanupDeviceD3D()
{
	delete food.image;
	delete player;
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		keys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		keys[VK_LBUTTON] = false;
		break;
	case WM_KEYDOWN:
		keys[LOWORD(wParam)] = true;
		break;
	case WM_KEYUP:
		keys[LOWORD(wParam)] = false;
		break;
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			GetClientRect(hWnd, &rc);
		}
		return 0;
	break;
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
}
