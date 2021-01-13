#pragma once

#include <string>
#include "Color.h"

class Renderer
{
public:
	void DrawStrokeText(int x, int y, RGBA* color, const char* str);
	void DrawNewText(int x, int y, RGBA* color, const char* str);
	void DrawRect(int x, int y, int w, int h, RGBA* color, int thickness);
	void DrawFilledRect(int x, int y, int w, int h, RGBA* color);
	void DrawCircleFilled(int x, int y, int radius, RGBA* color, int segments);
	void DrawCircle(int x, int y, int radius, RGBA* color, int segments, bool fill = false);
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color, float thickne);
	void DrawTriangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color);
	void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness);
	void DrawBox(int x1, int y1, int bx, int by, RGBA* color, int thickness);
	void DrawBottomHealth(int x, int y, int w, int h, int borderPx, RGBA* color);
	void DrawCornerBox(int x, int y, int w, int h, int borderPx, RGBA* color);
	void DrawNormalBox(int x, int y, int w, int h, int borderPx, RGBA* color);
	void DrawFilledBox(int x, int y, int w, int h, int borderPx, RGBA* color);
	void DrawString(float fontSize, int x, int y, RGBA* color, bool bCenter, bool stroke, const char* pText/*, ...*/);

private:
	std::wstring MBytesToWString(const char* lpcszString);
	std::string WStringToUTF8(const wchar_t* lpwcszWString);
	std::string string_To_UTF8(const std::string& str);
};

inline Renderer Render;