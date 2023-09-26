// CpuSim.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "Architecture.h"
#include "Program.h"
#include "CPU.h"

const int reg_bits = 32;
typedef unsigned int reg_type;

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace riscv_sim;


	int a[5][6];
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 6; i++)
			a[j][i] = j * 6 + i;

	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 6; i++)
			cout << a[j][i] << endl;

	for (int j = 0; j < 5; j++)
		for (int i = 0 + j; i < 6 + j; i++)
			cout << a[j][i - j] << endl;

	string asm_prog;
	string s_ins;
	string filename;

	if (argc > 1)
	{
		filename = "C:\\data\\prog.dat";
		std::ifstream ifs(filename, std::ifstream::in);
		if (ifs.good() == false)
		{
			printf("Cannot find input file: %s.", filename.c_str());
			return 0;
		}
		DataCache::load(ifs);
		Program prog(ifs);

		// Run the program
		CPU cpu;
		cpu.run(prog);
	}
	else
	{
		s_ins = "0, 111\n";
		asm_prog = s_ins;
		s_ins = "\n";
		asm_prog += s_ins;
		s_ins = "\taddi\tR1, R0, 24\n";
		asm_prog += s_ins;
		s_ins = "\taddi \tR2, R0, 124\n";
		asm_prog += s_ins;
		s_ins = "\tfld\tF2, 200(R0)\n";
		asm_prog += s_ins;
		s_ins = "loop:\tfld 	F0, 0(R1)\n";
		asm_prog += s_ins;
		s_ins = "\tfmul \tF0, F0, F2\n";
		asm_prog += s_ins;
		s_ins = "\tfld\tF4, 0(R2)\n";
		asm_prog += s_ins;
		s_ins = "\tfadd \tF0, F0, F4\n";
		asm_prog += s_ins;
		s_ins = "\tfsd\tF0, 0(R2)\n";
		asm_prog += s_ins;
		s_ins = "\taddi\tR1, R1, -8\n";
		asm_prog += s_ins;
		s_ins = "\taddi\tR2, R2, -8\n";
		asm_prog += s_ins;
		s_ins = "\tbne\tR1, $0, loop\n";
		asm_prog += s_ins;

		// Load program and convert to machine code
		Program prog(asm_prog);

		// Run the program
		CPU cpu;
		cpu.run(prog);
	}

    return 0;
}



std::string outHex(reg_type val)
{
	char str[16];
	sprintf_s(str, "0x%08X", val);
	return str;
}
