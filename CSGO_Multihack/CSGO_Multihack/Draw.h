#pragma once
#include "d3d.h"

/*

Type: Namespace
Intended for: Simple set of functions to draw 2D figures on screen with D3D.

*/

namespace Draw
{
	struct CPOINT
	{
		long x;
		long y;
		D3DCOLOR color;

		CPOINT()
		{
			x = 0; y = 0; color = D3DCOLOR_RGBA(0, 255, 0, 255);
		}

		CPOINT(long _x, long _y, long _c = D3DCOLOR_RGBA(0,255,0,255))
		{
			x = _x; y = _y; color = _c;
		}

		CPOINT(const CPOINT &copy)
		{
			x = copy.x; y = copy.y; color = copy.color;
		}
	};

	class Pixels 
	{
	private:
		CPOINT* m_points;
		size_t m_size;
		LPDIRECT3DDEVICE9 m_pDevice;

	public:
		Pixels();
		Pixels(LPDIRECT3DDEVICE9 pDevice);
		Pixels(const Pixels &copy);
		~Pixels();

		size_t GetSize() const;
		const CPOINT& operator[](size_t n) const;
		void Draw() const;

		void __fastcall Push(POINT &p, D3DCOLOR col = D3DCOLOR_RGBA(0, 255, 0, 255));
		void __fastcall Push(CPOINT &p);
		void Clear();
		void SetDevice(LPDIRECT3DDEVICE9 pDevice);
	};

	void __fastcall Pixel(LPDIRECT3DDEVICE9 pDevice, int x, int y, D3DCOLOR color);
	void __fastcall FilledRect(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, D3DCOLOR color);
	void __fastcall OutlinedRect(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
	void __fastcall FilledSquareWithCenter(LPDIRECT3DDEVICE9 pDevice, int x, int y, int sideWidth, D3DCOLOR color);
	void __fastcall OutlinedSquareWithCenter(LPDIRECT3DDEVICE9 pDevice, int x, int y, int sideWidth, int thickness, D3DCOLOR color);
	void __fastcall FilledCircle(LPDIRECT3DDEVICE9 pDevice, int xc, int yc, int radius, D3DCOLOR color);
	void __fastcall OutlinedCircle(LPDIRECT3DDEVICE9 pDevice, int xc, int yc, int radius, int thickness, D3DCOLOR color);
	void __fastcall Line(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
	void __fastcall Cross(LPDIRECT3DDEVICE9 pDevice, int x, int y, int size, int thickness, D3DCOLOR color);
}