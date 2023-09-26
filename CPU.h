#ifndef _CPU_
#define _CPU_
#include "Architecture.h"
#include "InstructionCache.h"
#include "InstructionUnit.h"
#include "ArithmeticLogicUnit.h"
#include "CircularROB.h"
#include "CommonDataBus.h"
#include "Program.h"
#include "RegisterFile.h"

namespace riscv_sim
{
	class CPU
	{
	public:
		// Max number of instructions that can be fetched per clock cycle
		static int NF;
		// Max number of instructions that can be issued per clock cycle
		static int NW;
		// Max number WB common data buses
		static int NB;
		// Num entries in ROB
		static unsigned char NR;

		InstructionCache i_cache;
		InstructionUnit insUnit;
		ArithmeticLogicUnit INT_ALU;
		ArithmeticLogicUnit LoadSave_ALU;
		ArithmeticLogicUnit FPadd_ALU;
		ArithmeticLogicUnit FPmul_ALU;
		ArithmeticLogicUnit FPdiv_ALU;
		ArithmeticLogicUnit BU_ALU;
		CommonDataBus cdb;
		RegisterFile regFile;
		DataCache d_cache;
		CircularROB rob;

	public:
		CPU();
		void build();
		void run(Program& prog);
	};

}
#endif
//EOF
