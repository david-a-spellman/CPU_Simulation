#include "CPU.h"
#include <iostream>
#include "DataCache.h"
using namespace riscv_sim;
using namespace std;


int CPU::NF = 4;
int CPU::NW = 4;
int CPU::NB = 4;
unsigned char CPU::NR = 16;

CPU::CPU() :
	i_cache(),
	insUnit(NF, NW, i_cache, cdb),
	INT_ALU(4, 1, regFile, cdb),
	LoadSave_ALU(4, 1, regFile, cdb),
	FPadd_ALU(3, 3, regFile, cdb),
	FPmul_ALU(4, 4, regFile, cdb),
	FPdiv_ALU(2, 8, regFile, cdb),
	BU_ALU(1, 1, regFile, cdb),
	cdb(NW, NB),
	regFile(32),
	d_cache(),
	rob(NR, &cdb)
{
	build();
}

void CPU::build()
{
	// Attach the common data bus
	cdb.addALU(&INT_ALU, int_alu);
	cdb.addALU(&LoadSave_ALU, data_alu);
	cdb.addALU(&FPadd_ALU, float_add_alu);
	cdb.addALU(&FPmul_ALU, float_mult_alu);
	cdb.addALU(&FPdiv_ALU, float_div_alu);
	cdb.addALU(&BU_ALU, branch_alu);

	cdb.addROB(&rob);
	cdb.addRF(&regFile);
	cdb.addDC(&d_cache);
	DataCache::initialize();
}

void CPU::run(Program& prog)
{
	i_cache.load(prog);
	
	int num_inst = prog.getMachineCode().size();
	for (int i = 0; i < num_inst * 2; ++i) // Replace with while loop
	{
		// Must cycle components in reverse to simulate processing propagation correctly!

		INT_ALU.cycle();
		LoadSave_ALU.cycle();
		FPadd_ALU.cycle();
		FPmul_ALU.cycle();
		FPdiv_ALU.cycle();
		BU_ALU.cycle();

		insUnit.cycle();

		cout << "Clock cycle: " << i << std::endl;
		cout << insUnit << endl;
		cout << regFile << endl;
		cout << rob << endl;
	}
}
