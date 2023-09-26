#include "UnsignedIntMath.h"
using namespace uim;

// method computes the absolute value of (x - y)
unsigned int UnsignedIntMath :: subtract (unsigned int x, unsigned int y)
{
	unsigned int answer;
	if (y > x)
	{
		answer = (x - y);
		answer = (max - answer);
	}
	else
	{
		answer = (x - y);
	}
	return answer;
}

// EOF