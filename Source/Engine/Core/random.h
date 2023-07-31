#pragma once
#include<random>

namespace meow
{
	inline void seedRandom(unsigned int seed) { srand(seed); }

	inline int random() { return rand(); }
	inline int random(unsigned int max) { return rand() % max; }//return between 0 and 1 less than max
	inline int random(unsigned int min, unsigned int max) { return min + random(max - min + 1); } //return numbers in between min and max inclusive

	inline float randomf() { return random() / (float)RAND_MAX; };
	inline float randomf(float max) { return randomf() * max; };
	inline float randomf(float min, float max) { return min + randomf() * (max - min); };

}