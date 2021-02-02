#include "Draw.h"

inline int Sign(double x)
{
	if (x < 0)
		return -1;
	else
		return 1;
}


namespace Draw
{
	void __fastcall Pixel(LPDIRECT3DDEVICE9 pDevice, int x, int y, D3DCOLOR color)
	{
		D3DRECT rec;
		rec.x1 = x;
		rec.y1 = y;
		rec.x2 = x + 1;
		rec.y2 = y + 1;
		pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 1.0f, 0);
	}

	void __fastcall FilledRect(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, D3DCOLOR color)
	{
			D3DRECT rec;
			rec.x1 = x1;
			rec.y1 = y1;
			rec.x2 = x2 + 1;
			rec.y2 = y2 + 1;
			pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 1.0f, 0);
	}

	void __fastcall OutlinedRect(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
	{
		thickness--;
		if (thickness < 0)
			thickness = 0;

		FilledRect(pDevice, x1, y1, x2, y1 + thickness, color); //Upper side
		FilledRect(pDevice, x1, y1, x1 + thickness, y2, color); //Left side
		FilledRect(pDevice, x1, y2 - thickness, x2, y2, color); //Bottom side
		FilledRect(pDevice, x2 - thickness, y1, x2, y2, color); //Right side
	}

	void __fastcall FilledSquareWithCenter(LPDIRECT3DDEVICE9 pDevice, int x, int y, int sideWidth, D3DCOLOR color)
	{
		int x1 = 0, x2, y1 = 0, y2;
		x = x - 1;
		y = y - 1;
		if (sideWidth % 2 != 0)
		{
			x = x + 1;
			y = y + 1;
			x1 = 1;
			y1 = 1;
		}

		x1 = (x - (sideWidth / 2) + 1) - x1;
		if (x1 < 0)
			x1 = 0;
		y1 = (y - (sideWidth / 2) + 1) - y1;
		if (y1 < 0)
			y1 = 0;

		x2 = (x + sideWidth / 2) - 1;
		if (x1 < 0)
			x1 = 0;
		y2 = (y + sideWidth / 2) - 1;
		if (y1 < 0)
			y1 = 0;


		if ((x2 - x1) > sideWidth)
		{
			x1++;
			y1++;
		}

		if ((x2 - x1) < sideWidth)
		{
			x2++;
			y2++;
		}

		FilledRect(pDevice, x1, y1, x2, y2, color);
	}

	void __fastcall OutlinedSquareWithCenter(LPDIRECT3DDEVICE9 pDevice, int x, int y, int sideWidth, int thickness, D3DCOLOR color)
	{
		int x1 = 0, x2, y1 = 0, y2;
		x = x - 1;
		y = y - 1;
		if (sideWidth % 2 != 0)
		{
			x = x + 1;
			y = y + 1;
			x1 = 1;
			y1 = 1;
		}

		x1 = (x - (sideWidth / 2) + 1) - x1;
		if (x1 < 0)
			x1 = 0;
		y1 = (y - (sideWidth / 2) + 1) - y1;
		if (y1 < 0)
			y1 = 0;

		x2 = (x + sideWidth / 2) - 1;
		if (x1 < 0)
			x1 = 0;
		y2 = (y + sideWidth / 2) - 1;
		if (y1 < 0)
			y1 = 0;

		if ((x2 - x1) > sideWidth)
		{
			x1++;
			y1++;
		}

		if ((x2 - x1) < sideWidth)
		{
			x2++;
			y2++;
		}
		
		if (thickness >= sideWidth)
			thickness = sideWidth - 1;

		if (thickness < 1)
			thickness = 1;

		OutlinedRect(pDevice, x1, y1, x2, y2, thickness, color);
	}

	void __fastcall FilledCircle(LPDIRECT3DDEVICE9 pDevice, int xc, int yc, int radius, D3DCOLOR color)
	{
		int x, y, d;
		xc++; yc++;
		int ri = radius - 1;
		if (ri < 2)
			ri = 2;
		for (int i = ri; i > 1; i--)
		{
			x = 0, y = i, d = 2 * (1 - i);
			FilledSquareWithCenter(pDevice, xc + 1, yc, 2, color); 
			FilledSquareWithCenter(pDevice, xc + 1, yc, 2, color); 
			while (y > 0)
			{
				FilledSquareWithCenter(pDevice, xc + x, yc + y, 2, color); 
				FilledSquareWithCenter(pDevice, xc + x, yc - y, 2, color); 
				FilledSquareWithCenter(pDevice, xc - x, yc + y, 2, color); 
				FilledSquareWithCenter(pDevice, xc - x, yc - y, 2, color); 
				if (d + y > 0)
				{
					y -= 1;
					d -= (2 * y) - 1;
				}
				if (x > d)
				{
					x += 1;
					d += (2 * x) + 1;
				}
			}
		}

		if(radius < 3)
			FilledSquareWithCenter(pDevice, xc, yc, 2, color);
		else
			FilledSquareWithCenter(pDevice, xc, yc, 3, color);
	}

	void __fastcall OutlinedCircle(LPDIRECT3DDEVICE9 pDevice, int xc, int yc, int radius, int thickness, D3DCOLOR color)
	{
		int x, y, d;
		if (thickness < 1)
			thickness = 1;

		x = 0, y = radius, d = 2 * (1 - radius);
		Pixel(pDevice, xc + radius, yc, color); FilledSquareWithCenter(pDevice, xc + radius, yc, thickness, color);
		Pixel(pDevice, xc - radius, yc, color); FilledSquareWithCenter(pDevice, xc - radius, yc, thickness, color);
		while (y > 0)
		{
			FilledSquareWithCenter(pDevice, xc + x, yc + y, thickness, color);
			FilledSquareWithCenter(pDevice, xc + x, yc - y, thickness, color);
			FilledSquareWithCenter(pDevice, xc - x, yc + y, thickness, color);
			FilledSquareWithCenter(pDevice, xc - x, yc - y, thickness, color);
			if (d + y > 0)
			{
				y -= 1;
				d -= (2 * y) - 1;
			}
			if (x > d)
			{
				x += 1;
				d += (2 * x) + 1;
			}
		}
	}



	void __fastcall Line(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
	{
		if (thickness < 1)
			thickness = 1;

		double _x1 = x1;
		double _y1 = y1;
		double _x2 = x2;
		double _y2 = y2;
		double dy = y2 - y1;
		double dx = x2 - x1;

		if (fabs(dy) > fabs(dx))
		{
			for (y2 = (int)_y1; y2 != (int)_y2; y2 += Sign(dy))
			{
				x2 = (int)(_x1 + (y2 - _y1) * dx / dy);
				FilledSquareWithCenter(pDevice, x2, y2, thickness, color);

			}
		}

		else
		{
			for (x2 = (int)_x1; x2 != (int)_x2; x2 += Sign(dx))
			{
				y2 = (int)(_y1 + (x2 - _x1) * dy / dx);
				FilledSquareWithCenter(pDevice, x2, y2, thickness, color);
			}
		}

		FilledSquareWithCenter(pDevice, x2, y2, thickness, color);
	}

	void __fastcall Cross(LPDIRECT3DDEVICE9 pDevice, int x, int y, int size, int thickness, D3DCOLOR color)
	{
		int l1_x1, l1_y1, l1_x2, l1_y2, l2_x1, l2_y1, l2_x2, l2_y2;

		l1_x1 = x; l2_y1 = y; l1_x2 = x; l2_y2 = y;
		l1_y1 = y - size; l1_y2 = y + size;
		l2_x1 = x - size; l2_x2 = x + size;

		if (l1_y1 < 0)
			l1_y1 = 0;
		if (l2_x1 < 0)
			l2_x1 = 0;

		Line(pDevice, l1_x1, l1_y1, l1_x2, l1_y2, thickness, color);
		Line(pDevice, l2_x1, l2_y1, l2_x2, l2_y2, thickness, color);
	}

	Pixels::Pixels()
	{
		m_pDevice = nullptr;
		m_size = 0;
		m_points = nullptr;
	}

	Pixels::Pixels(LPDIRECT3DDEVICE9 pDevice)
	{
		m_pDevice = pDevice;
		m_size = 0;
		m_points = nullptr;
	}

	Pixels::Pixels(const Pixels &copy)
	{
		size_t size = copy.m_size;
		m_pDevice = copy.m_pDevice;
		m_size = 0;

		for (size_t i = 0; i < size; i++)
			Push(copy.m_points[i]);
	}

	Pixels::~Pixels()
	{
		Clear();
	}

	void __fastcall Pixels::Push(POINT &p, D3DCOLOR col)
	{
		CPOINT* p_tmp = m_points;

		size_t newsize = m_size + 1;

		m_points = new CPOINT[newsize];

		for (size_t i = 0; i < m_size; i++)
		{
			m_points[i].x = p_tmp[i].x;
			m_points[i].y = p_tmp[i].y;
			m_points[i].y = p_tmp[i].color;
		}

		m_points[m_size].x = p.x;
		m_points[m_size].y = p.y;
		m_points[m_size].color = col;

		if (m_size > 0)
			delete[] p_tmp;

		m_size = newsize;
	}

	void __fastcall Pixels::Push(CPOINT &p)
	{
		CPOINT* p_tmp = m_points;

		size_t newsize = m_size + 1;

		m_points = new CPOINT[newsize];

		for (size_t i = 0; i < m_size; i++)
		{
			m_points[i].x = p_tmp[i].x;
			m_points[i].y = p_tmp[i].y;
			m_points[i].y = p_tmp[i].color;
		}

		m_points[m_size].x = p.x;
		m_points[m_size].y = p.y;
		m_points[m_size].color = p.color;

		if (m_size > 0)
			delete[] p_tmp;

		m_size = newsize;
	}

	void Pixels::Clear()
	{
		if (m_size > 0)
		{
			delete[] m_points;
			m_size = 0;
		}
	}

	void Pixels::SetDevice(LPDIRECT3DDEVICE9 pDevice)
	{
		m_pDevice = pDevice;
	}

	void Pixels::Draw() const
	{
		if (m_pDevice == nullptr)
			return;
		for (size_t i = 0; i < m_size; i++)
			Pixel(m_pDevice, m_points[i].x, m_points[i].y, m_points[i].color);
	}

	size_t Pixels::GetSize() const
	{
		return m_size;
	}

	const CPOINT& Pixels::operator[](size_t n) const
	{
		return m_points[n];
	}
} 

