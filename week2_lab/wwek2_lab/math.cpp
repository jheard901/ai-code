
#include "math.h"

//create a random float between 0 - 1
float frn() {
	float randNum = float(rand());
	return randNum / RAND_MAX;
}

float GetRandomFloat(float a, float b)
{
	if (a >= b) // bad input
		return a;

	return (frn() * (b - a)) + a;
}

int GetRandomInt(float a, float b)
{
	if (a >= b) // bad input
		return a;

	return int((frn() * (b - a)) + a);
}