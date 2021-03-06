#include "../Header.h"
#include "d3d9_draw.h"

void D3D9_Draw::Line(LPDIRECT3DDEVICE9 device, D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, float width, DWORD color, bool antialias)
{
	ID3DXLine* line;
	D3DXCreateLine(device, &line);
	D3DXVECTOR2 linePos[] = { startPos, endPos };
	line->SetWidth(width);
	line->SetAntialias(antialias);
	line->Begin();
	line->Draw(linePos, 2, color);
	line->End();
	line->Release();
	return;
}

void D3D9_Draw::Box(LPDIRECT3DDEVICE9 device, D3DXVECTOR2 Pos, float width, float height, float widthpx, DWORD color, bool antialias)
{
	ID3DXLine* line;
	D3DXCreateLine(device, &line);
	D3DXVECTOR2 points1[2];
	points1[0] = D3DXVECTOR2(Pos.x - widthpx / 2, Pos.y);
	points1[1] = D3DXVECTOR2(Pos.x + width + widthpx / 2, Pos.y);
	D3DXVECTOR2 points2[2];
	points2[0] = D3DXVECTOR2(Pos.x + width, Pos.y + widthpx / 2);
	points2[1] = D3DXVECTOR2(Pos.x + width, Pos.y + height - widthpx / 2);
	D3DXVECTOR2 points3[2];
	points3[0] = D3DXVECTOR2(Pos.x - widthpx / 2, Pos.y + height);
	points3[1] = D3DXVECTOR2(Pos.x + width + widthpx / 2, Pos.y + height);
	D3DXVECTOR2 points4[2];
	points4[0] = D3DXVECTOR2(Pos.x, Pos.y + widthpx / 2);
	points4[1] = D3DXVECTOR2(Pos.x, Pos.y + height - widthpx / 2);
	line->SetWidth(widthpx);
	line->SetAntialias(antialias);
	line->Begin();
	line->Draw(points1, 2, color);
	line->Draw(points2, 2, color);
	line->Draw(points3, 2, color);
	line->Draw(points4, 2, color);
	line->End();
	line->Release();
	return;
}

void D3D9_Draw::BoxFilled(LPDIRECT3DDEVICE9 device, D3DXVECTOR2 Pos, float width, float height, DWORD color, bool antialias)
{
	ID3DXLine* line;
	D3DXCreateLine(device, &line);
	D3DXVECTOR2 points[2];
	points[0] = D3DXVECTOR2(Pos.x + width / 2, Pos.y);
	points[1] = D3DXVECTOR2(Pos.x + width / 2, Pos.y + height);

	line->SetWidth(width);
	line->SetAntialias(antialias);
	line->Begin();
	line->Draw(points, 2, color);
	line->End();
	line->Release();
	return;
}

void D3D9_Draw::String(LONG x, LONG y,  DWORD color, LPD3DXFONT& g_pFont, const wstring str)
{
	RECT fontPos = { x, y, x + 120, y + 16 };
	g_pFont->DrawTextW(nullptr, str.c_str(), -1, &fontPos, DT_LEFT | DT_NOCLIP, color);
	return;
}