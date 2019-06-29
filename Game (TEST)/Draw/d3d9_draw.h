#pragma once

#include "GameSprite/GameSprite.h"

namespace D3D9_Draw
{
	void Line(LPDIRECT3DDEVICE9 device, D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, float width, DWORD color, bool antialias);
	void Box(LPDIRECT3DDEVICE9 device, D3DXVECTOR2 Pos, float width, float height, float widthpx, DWORD color, bool antialias);
	void BoxFilled(LPDIRECT3DDEVICE9 device, D3DXVECTOR2 Pos, float width, float height, DWORD color, bool antialias);
	void String(LONG x, LONG y, DWORD color, LPD3DXFONT& g_pFont, const wstring str);
}