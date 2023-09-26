#ifndef _InstructionCache_
#define _InstructionCache_
#include <vector>
#include "Architecture.h"
#include "Program.h"


class InstructionCache
{
public:
	std::vector<reg_type> mem;
public:
	InstructionCache();
	void load(Program& prog);
	std::vector<reg_type> read(reg_type address, int bw);
};


#endif
// EOF
