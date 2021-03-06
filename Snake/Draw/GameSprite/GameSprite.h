#ifndef GAMESPRITE_H
#define GAMESPRITE_H

//Texture and Sprites and other functions.
#include <d3dx9.h>
#include <string>

class GameSprite
{
public:
	//Constructors and Destructor
	GameSprite();
	~GameSprite();

	//Sprite Functions
	bool Initialize(LPDIRECT3DDEVICE9 device, std::wstring file, int width, int height);
	bool IsInitialized();
	virtual void Draw(float x, float y, float z);
private:
	//Pointers
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	//Attributes
	D3DXVECTOR3 position;
	D3DCOLOR color;
	bool initialized;
};

#endif /* GAMESPRITE_H */