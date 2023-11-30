#ifndef FIG_H
#define FIG_H

#include <cmath>
#include "Figure.h"

class Myfig : public Figure
{
public:
	Myfig(int size = 12, int offsetX = 500, int offsetY = 300) : Figure(size, offsetX, offsetY)
	{
	}
	void Draw(HDC dc)
	{
		MoveToEx(dc, offsetX - 5*size, offsetY - 4*size, NULL);
		LineTo(dc, offsetX - 4*size, offsetY - 5*size);
		LineTo(dc, offsetX, offsetY - size);
		LineTo(dc, offsetX + 4*size, offsetY - 5*size);
		LineTo(dc, offsetX + 5*size, offsetY - 4*size);
		LineTo(dc, offsetX + size, offsetY);
		LineTo(dc, offsetX + 5*size, offsetY + 4 * size);
		LineTo(dc, offsetX + 4 * size, offsetY + 5 * size);
		LineTo(dc, offsetX, offsetY + size);
		LineTo(dc, offsetX - 4 * size, offsetY + 5 * size);
		LineTo(dc, offsetX - 5 * size, offsetY + 4 * size);
		LineTo(dc, offsetX - size, offsetY);
		LineTo(dc, offsetX - 5 * size, offsetY - 4 * size);
	}
	void InnerCheck(int X, int Y) {
		if (InnerPoint(X, Y)) StartDragging(X, Y);
	}
	bool InnerPoint(int X, int Y)
	{
		int a1 = 1, b1 = 1, c1 = -(offsetY + offsetX);
		int a2 = -1, b2 = 1, c2 = offsetX - offsetY;
		float lh = size * sqrt(2) / 2;
		float lc = 6.37 * size;
		float l1 = abs(a1 * X + b1 * Y + c1) / sqrt(a1 * a1 + b1 * b1);
		float l2 = abs(a2 * X + b2 * Y + c2) / sqrt(a2 * a2 + b2 * b2);
		float l3 = sqrt((offsetX - X)*(offsetX - X) + (offsetY - Y)*(offsetY - Y));
		return (l1 < lh || l2 < lh) && l3 < lc;

	}
};

#endif FIG_H
