#ifndef _INSTRUCTIONUNIT_
#define _INSTRUCTIONUNIT_
#include "Architecture.h"
#include "Register.h"
#include "Instruction.h"
#include "InstructionCache.h"
#include "BranchControler.h"
#include "CommonDataBus.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>

namespace riscv_sim
{
	class InstructionUnit : public Component
	{
	private:
		int NF;
		int NW;
		Register PC;
		InstructionCache* i_cache;
		std::vector<reg_type> instructionQueue; // By using a vector, the index is effectly the instruction address/4
		int nextDecodeIns;
		std::vector<reg_type> dispatchQueue;
		int nextDispatchIns;
		std::vector<bool> issued;
		BranchControler branchUnit;
		bool branched;
		int branchOffset;
		CommonDataBus* cdb;
	public:
		InstructionUnit(int nf, int nw, InstructionCache& i_cache, CommonDataBus& bus);
		void cycle();
		reg_type getOutput(int i = 0) const { return 0; }
		size_t numOutputs() const { return 1; }
		void fetch();
		void decode();
		bool dispatch();
		reg_type rename_registers(Instruction ins);
		std::string rename_register(std::string reg);
		void free_register(std::string rename);
		bool allocate_register(std::string reg);
		friend std::ostream & operator << (std::ostream &out, const InstructionUnit &r);
		std :: vector <reg_type> addToROB(Instruction inst, reg_type address);
		int hasRawDependancy(unsigned char reg);
	};
}

#endif