#include "InstructionCache.h"


InstructionCache::InstructionCache() :
	mem(10000, 0)
{
}

void InstructionCache::load(Program& p)
{
	std::vector<reg_type>& prog = p.getMachineCode();
	for (size_t i = 0; i < prog.size(); ++i)
		mem.at(i) = prog.at(i);
}

std::vector<reg_type> InstructionCache::read(reg_type address, int bw)
{
	std::vector<reg_type> ins;
	reg_type start = address / 4;
	for (int i = 0; i < bw; ++i)
		ins.push_back(mem.at(start + i));
	return ins;
}
