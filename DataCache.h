#ifndef _DataCache_
#define _DataCache_
#include <vector>
#include <fstream>

namespace riscv_sim
{
	class DataCache
	{
	private:
		static std :: vector <unsigned int> memory;
	public:
		static void initialize ();
		static void initialize (std :: vector <unsigned int>& vals);
		static void load(std::ifstream& file);
		static unsigned int loadFloatingPoint (unsigned int address);
		static void storeFloatingPoint (unsigned int address, unsigned int value);
	};
}
#endif
// EOF