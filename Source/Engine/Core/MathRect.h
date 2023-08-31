#pragma once
namespace meow {
	class MathRect
	{
	public:
		int x, y, w, h;


		MathRect() : x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 } {}
		MathRect(int x, int y, int w, int h) : x{ x }, y{ y }, w{ w }, h{ h } {}

		int operator [] (size_t index) const { return (&x)[index]; }
		int& operator[] (size_t index) { return (&x)[index]; }

	};
}