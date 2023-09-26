#ifndef _UNSIGNEDINTMATH_
#define _UNSIGNEDINTMATH_


namespace uim
{
	class UnsignedIntMath
	{
		private:
		static const unsigned int max = 0xFFFFFFFF;

		// method computes the absolute value of (x - y)
		static unsigned int subtract (unsigned int x, unsigned int y);
	};
}

#endif
// EOF