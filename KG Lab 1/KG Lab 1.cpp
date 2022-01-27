#include <windows.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

HWND console = GetConsoleWindow();
HDC hdc = GetDC(console);
COLORREF white = RGB(255, 255, 255);
COLORREF red = RGB(255, 0, 0);
COLORREF green = RGB(0, 255, 0);
COLORREF blue = RGB(0, 0, 255);

void line(int x1, int y1, int x2, int y2, COLORREF color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (dx != 0)
	{
		float k = dy / dx;
		int x = x1;
		int y = y1;
		while (x <= x2)
		{
			SetPixel(hdc, x, y, color);
			y += (int)k;
			x++;
		}
	}
	else
	{
		float k = dy / (dx + 0.1);
		int x = x1;
		int y = y1;
		while (x <= x2)
		{
			SetPixel(hdc, x, y, color);
			y += (int)k;
			x++;
		}
	}
}

void brezen4(int x1, int y1, int x2, int y2, COLORREF color)
{
	int dX, dY, sX, sY, e;
	dX = abs(x2 - x1);
	dY = abs(y2 - y1);
	if (x1 < x2)
		sX = 1;
	else
		sX = -1;
	if (y1 < y2)
		sY = 1;
	else
		sY = -1;
	e = dX - dY;
	SetPixel(hdc, x2, y2, color);
	while ((x1 != x2) || (y1 != y2))
	{
		SetPixel(hdc, x1, y1, color);
		int e2 = e * 2;
		if (e2 > -dY)
		{
			e -= dY;
			x1 += sX;
		}
		if (e2 < dX)
		{
			e += dX;
			y1 += sY;
		}
	}
}

void drawCircle(int x0, int y0, int r, COLORREF Color = RGB(255, 255, 255)) {
	int x = 0;
	int y = r;
	int d = 1 - 2 * r;
	int e = 0;
	while (y >= 0) {
		SetPixel(hdc, x0 + x, y0 + y, Color);
		SetPixel(hdc, x0 + x, y0 - y, Color);
		SetPixel(hdc, x0 - x, y0 + y, Color);
		SetPixel(hdc, x0 - x, y0 - y, Color);
		e = 2 * (d + y) - 1;
		if (d < 0 && e <= 0) {
			++x;
			d += 2 * x + 1;
			continue;
		}
		e = 2 * (d - x) - 1;
		if (d > 0 && e > 0) {
			--y;
			d += 1 - 2 * y;
			continue;
		}
		++x;
		d += 2 * (x - y);
		--y;
	}
}

int LineFill(int x, int y, int dir, int PrevXl, int PrevXr, COLORREF BorderColor, COLORREF Color)
{
	int xl = x, xr = x, c;
	do c = GetPixel(hdc, --xl, y); while ((c != BorderColor) && (c != Color));
	do c = GetPixel(hdc, ++xr, y); while ((c != BorderColor) && (c != Color));
	xl++; xr--;
	line(xl, y, xr, y, Color);
	for (x = xl; x <= xr; x++)
	{
		c = GetPixel(hdc, x, y + dir);
		if ((c != BorderColor) && (c != Color)) x = LineFill(x, y + dir, dir, xl, xr, BorderColor, Color);
	}
	for (x = xl; x < PrevXl; x++)
	{
		c = GetPixel(hdc, x, y - dir);
		if ((c != BorderColor) && (c != Color)) x = LineFill(x, y - dir, -dir, xl, xr, BorderColor, Color);
	}
	for (x = PrevXr; x < xr; x++)
	{
		c = GetPixel(hdc, x, y - dir);
		if ((c != BorderColor) && (c != Color)) x = LineFill(x, y - dir, -dir, xl, xr, BorderColor, Color);
	}
	return xr;
}

int main()
{
	line(400 + rand() % 500, 100 + rand() % 200, 400 + rand() % 500, 100 + rand() % 200, red);
	brezen4(400 + rand() % 500, 100 + rand() % 200, 400 + rand() % 500, 100 + rand() % 200, green);
	int circle1X, circle1Y, circle2X, circle2Y, radius;
	double dist;
	circle1X = 100 + rand() % 200;
	circle1Y = 100 + rand() % 200;
	circle2X = 100 + rand() % 200;
	circle2Y = 100 + rand() % 200;
	radius = 50 + rand() % 10;
	dist = sqrt(pow(circle1X - circle2X, 2) + pow(circle1Y - circle2Y, 2));
	if (int(dist) < (2 * radius) && int(dist) > radius)
	{
		drawCircle(circle1X, circle1Y, radius);
		drawCircle(circle2X, circle2Y, radius);
		LineFill((circle1X + circle2X) / 2, (circle1Y + circle2Y) / 2, 1, (circle1X + circle2X) / 2, (circle1X + circle2X) / 2, white, blue);
		LineFill(circle1X, circle1Y, 1, circle1X, circle1X, white, red);
		LineFill(circle2X, circle2Y, 1, circle2X, circle2X, white, green);
	}
}