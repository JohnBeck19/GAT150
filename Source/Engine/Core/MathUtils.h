#pragma once
#include <cmath>
#include <utility>
namespace meow
{
	constexpr float Pi = 3.14159265359f;
	constexpr float TwoPi = Pi * 2;
	constexpr float HalfPi = Pi * .5;

	constexpr float RadToDeg(float radians) { return radians * (100.0f / Pi); };
	constexpr float DegToRad(float degrees) { return degrees * (Pi / 180.0f); };

	constexpr int Wrap(int value, int max) {

		return (value % max) + ((value < 0) ? max : 0);
	}
	inline float Wrap(float value, float max)
	{
		return std::fmod(value, max) + ((value < 0) ? max : 0);
	}
	inline int Max(int a, int b) {
	
		return (a > b) ? a : b;
	}
	inline float Max(float a, float b) {

		return (a > b) ? a : b;
	}
	template <typename T>
	inline T Max(T a, T b) {

		return (a > b) ? a : b;
	}
	template <typename T>
	inline T Min(T a, T b) {

		return (a < b) ? a : b;
	}
	template <typename T>
	inline T Clamp(T value, T min, T max) {
		
		if (min > max) std::swap(min, max);
		return (value < min) ? min : (value > max) ? max : value;
	}
	template<typename T> //T is between 0 and 1
	constexpr T Lerp(const T& a, const T& b, float t)
	{
		return (a * (1.0f - t)) + (b * t);
	}
}