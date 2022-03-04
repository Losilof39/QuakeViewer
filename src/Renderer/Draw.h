#pragma once
#include "quakedef.h"

extern uint32_t* backbuffer;

void Fill(int width, int height, uint32_t color);
void Pixel(int x, int y, uint32_t color);
void Rectangle(int x, int y, int width, int height, uint32_t color);
void Line(int x1, int y1, int x2, int y2, uint32_t color);
void ClippedLine(int x1, int y1, int x2, int y2, uint32_t color);
void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
void ClippedTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);