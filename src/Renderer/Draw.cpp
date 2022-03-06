#include "Draw.h"

#define INSIDE 0
#define RIGHT 1
#define LEFT 3
#define UP 2
#define DOWN 4

void Fill(int width, int height, uint32_t color)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			backbuffer[x + width * y] = color;
		}

	}
}

void Pixel(int x, int y, uint32_t color)
{
	if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
		backbuffer[x + WIDTH * y] = color;
}

void DrawRectangle(int x, int y, int width, int height, uint32_t color)
{
	int start = x + y * WIDTH;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			backbuffer[start + x + y * WIDTH] = color;
		}
	}
}

// https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
void DrawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		Pixel(x, y, color);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			Pixel(x, y, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		Pixel(x, y, color);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			Pixel(x, y, color);
		}
	}
}

void DrawLine(vec4_t p1, vec4_t p2, uint32_t color)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = p2.x - p1.x; dy = p2.y - p1.y;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = p1.x; y = p1.y; xe = p2.x;
		}
		else
		{
			x = p2.x; y = p2.y; xe = p1.x;
		}

		Pixel(x, y, color);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			Pixel(x, y, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = p1.x; y = p1.y; ye = p2.y;
		}
		else
		{
			x = p2.x; y = p2.y; ye = p1.y;
		}

		Pixel(x, y, color);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			Pixel(x, y, color);
		}
	}
}

// draw a line clipped between the window rect, but it rejects
// a line that has 2 points outside and still crosses the window rect

void DrawClippedLine(int x1, int y1, int x2, int y2, uint32_t color)
{
	int outside1 = INSIDE;
	int outside2 = INSIDE;

	// check if first point is outside
	if (x1 >= WIDTH)
	{
		outside1 = RIGHT;
	}
	else if (x1 < 0)
	{
		outside1 = LEFT;
	}
	
	if (y1 >= HEIGHT)
	{
		outside1 = DOWN;
	}
	else if (y1 < 0)
	{
		outside1 = UP;
	}

	// check if second point is outside
	if (x2 >= WIDTH)
	{
		outside2 = RIGHT;
	}
	else if (x2 < 0)
	{
		outside2 = LEFT;
	}
	
	if (y2 >= HEIGHT)
	{
		outside2 = DOWN;
	}
	if (y2 < 0)
	{
		outside2 = UP;
	}

	int x, y;

	// find out which one is inside and outside and clip accordingly
	if (outside1 == INSIDE && outside2 == INSIDE)
	{
		DrawLine(x1, y1, x2, y2, color);
		return;
	}
	else
	{
		int which = outside1 > outside2 ? outside1 : outside2;

		if (which == UP)
		{
			x = x1 + (x2 - x1) * (0 - y1) / (y2 - y1);
			y = 0;
		}
		if (which == DOWN)
		{
			x = x1 + (x2 - x1) * (HEIGHT - 1 - y1) / (y2 - y1);
			y = HEIGHT - 1;
		}
		if (which == RIGHT)
		{
			y = y1 + (y2 - y1) * (WIDTH - 1 - x1) / (x2 - x1);
			x = WIDTH - 1;
		}
		if (which == LEFT)
		{
			y = y1 + (y2 - y1) * (0 - x1) / (x2 - x1);
			x = 0;
		}

	}
	
	if(outside1 == INSIDE)
		DrawLine(x1, y1, x, y, color);
	else
		DrawLine(x, y, x2, y2, color);

}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void DrawTriangle(vec4_t p1, vec4_t p2, vec4_t p3, uint32_t color)
{
	DrawLine(p1, p2, color);
	DrawLine(p2, p3, color);
	DrawLine(p3, p1, color);
}

void DrawClippedTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
	DrawClippedLine(x1, y1, x2, y2, color);
	DrawClippedLine(x2, y2, x3, y3, color);
	DrawClippedLine(x3, y3, x1, y1, color);
}

// https://www.avrfreaks.net/sites/default/files/triangles.c
void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color, bool lerp)
{
	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	//auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Pixel(i, ny, color); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		
		float w1, w2, w3 = 0.0f;

		for (int x = minx; x <= maxx; x++)
		{
			if (lerp)
			{
				BarycentricCoords(x, y, x1, y1, x2, y2, x3, y3, &w1, &w2, &w3);

				float r = 255 * w1;
				float g = 255 * w2;
				float b = 255 * w3;

				uint32_t val = (int)r << 16 | (int)g << 8 | (int)b;
				Pixel(x, y, val);
				continue;
			}
			Pixel(x, y, color);
		}
		//drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		//drawline(minx, maxx, y);

		float w1, w2, w3;

		for (int x = minx; x <= maxx; x++)
		{
			if (lerp)
			{
				BarycentricCoords(x, y, x1, y1, x2, y2, x3, y3, &w1, &w2, &w3);

				float r = 255 * w1;
				float g = 255 * w2;
				float b = 255 * w3;

				uint32_t val = (int)r << 16 | (int)g << 8 | (int)b;
				Pixel(x, y, val);
				continue;
			}
			Pixel(x, y, color);
		}

		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
}