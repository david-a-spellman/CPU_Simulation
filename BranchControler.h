#ifndef _BranchControler_
#define _BranchControler_
#include "UnsignedIntMath.h"
#include <vector>
using namespace uim;

namespace riscv_sim
{
	class BranchControler
	{
	private:
		unsigned char size;
		std::vector<unsigned int> addresses;
		std::vector<bool> predictions;
		bool predict (unsigned char index);
	public:
		BranchControler (unsigned char s = 0);
		void transition (unsigned int address, bool prediction, unsigned int target_address);
		bool makePrediction (unsigned int address);
		unsigned int getAddress (unsigned int address);
	};
}
#endif
// EOF