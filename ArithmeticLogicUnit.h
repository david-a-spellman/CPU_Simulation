#ifndef _ArithmeticLogicUnit_
#define _ArithmeticLogicUnit_
#include <string>
#include "UnsignedIntMath.h"
#include "Instruction.h"
#include "Register.h"
#include "RegisterFile.h"
#include "CommonDataBus.h"
using namespace uim;

namespace riscv_sim
{
	class CommonDataBus;


	class ReservationStation : public Component
	{
	public:
		ReservationStation() : Component("RegStation"), IR ("IR"), A ("A"), B ("B"), instID(0), occupied (false), isReady (false) {}
		void cycle()
		{
			IR.cycle();
			A.cycle();
			B.cycle();
		}
		reg_type instID;
		Register IR;
		Register A;
		bool isDependantA;
		bool isDependantB;
		Register B;
		bool occupied;
		bool isReady;
	};

	class ArithmeticLogicUnit : public Component
	{
	private:
		std::vector<ReservationStation> regStations;
		int numCycles;
		int curCycle;
		ReservationStation* curStation;
		RegisterFile* regFile;
		CommonDataBus* cdb;
	public:
		ArithmeticLogicUnit (int numStations, int cycles, RegisterFile& rf, CommonDataBus& bus, std :: string n = "ALU");
		std::vector<ReservationStation>& getStations() { return regStations; }
		void cycle();
		void issue(int resStationNum, reg_type ins_ID, reg_type ins_code, std :: vector <reg_type> data_deps);
		bool isBusy();
		bool hasOutput();
		unsigned int getOutput (Register& instruction, Register& a, Register& b);
		unsigned int calculateAddress (Register& reg, Instruction& inst);
		unsigned int addImmediate (Register& reg, Instruction& inst);
		unsigned int add (Register& reg1, Register& reg2);
		unsigned int addFloat (Register& reg1, Register& reg2);
		unsigned int subFloat (Register& reg1, Register& reg2);
		unsigned int multFloat (Register& reg1, Register& reg2);
		unsigned int divFloat (Register& reg1, Register& reg2);
		unsigned int branchNotEqual (Register& reg1, Register& reg2, Instruction& inst);
	};
}
#endif
// EOF