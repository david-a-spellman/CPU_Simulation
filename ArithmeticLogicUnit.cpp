#include <string>
#include "Register.h"
#include "ArithmeticLogicUnit.h"
#include "Instruction.h"
#include "DataCache.h"
using namespace riscv_sim;

ArithmeticLogicUnit :: ArithmeticLogicUnit (int numStations, int cycles, RegisterFile& rf, CommonDataBus& bus, std :: string n) :
	Component(n), regStations(numStations), numCycles(cycles), curCycle(0), curStation(nullptr), regFile(&rf), cdb(&bus) {}

void ArithmeticLogicUnit::cycle()
{
	// If not busy, check for a ready reservation station
	if (!isBusy())
	{
		for (size_t i = 0; i < regStations.size(); ++i)
			if (regStations.at(i).isReady)
			{
				curStation = &regStations.at(i);
				break;
			}
	}

	// Output only available on cycle == numCycles
	if (isBusy())
	{
		curCycle++;
		if (curCycle == numCycles)
		{
			// Send the result on the bus with the address
			reg_type code = curStation->IR.getOutput();
			Instruction ins(code);
			cdb->rob->addEntry(curStation->instID, ins.rd, getOutput(curStation->IR, curStation->A, curStation->B), true);
			curStation = nullptr;
			curCycle = 0;
		}
	}
	else
		curCycle = 0;

	if (curCycle > numCycles)
		printf("ERROR: ALU went too many cycles!!!");
}

// Dispatcher decides which res station
void ArithmeticLogicUnit::issue(int resStationNum, reg_type ins_ID, reg_type ins_code, std :: vector <reg_type> data_deps)
{
	regStations.at(resStationNum).instID = ins_ID;

	// Need to write issued instrution to IR of the target res station
	regStations.at(resStationNum).IR.set (ins_code);

	// Need to get operands from RegFile if possible
	Instruction ins(ins_code);
	regStations.at(resStationNum).A.setInput(regFile->getReg(ins.rs1));
	regStations.at(resStationNum).isDependantA = false;
	regStations.at(resStationNum).isDependantB = false;
	// if there is a data dependancy we cannot read the value from the register so we have to overwrite it and mark the reservation station as holding a ROB reference
	if (data_deps.size() == 1)
	{
		if (data_deps.at (0) == regStations.at(resStationNum).A.getValue ())
		{
		regStations.at(resStationNum).isDependantA = true;
		} else {
			regStations.at(resStationNum).isDependantB = true;
		}
	} else if (data_deps.size() > 1)
	{
		if (data_deps.at(0) == regStations.at(resStationNum).A.getValue())
		{
			regStations.at(resStationNum).isDependantA = true;
			regStations.at(resStationNum).isDependantB = true;
		} else {
			regStations.at(resStationNum).isDependantA = true;
			regStations.at(resStationNum).isDependantB = true;
		}
	}
	switch (ins.f)
	{
	case R:
	case S:
		regStations.at(resStationNum).B.setInput(regFile->getReg(ins.rs1));
		break;
	};

	// Need to track dependencies from operands to other instructions (if any)
	bool hasDepA = regStations.at(resStationNum).isDependantA;
	bool hasDepB = regStations.at(resStationNum).isDependantB;
	regStations.at(resStationNum).isReady = (!hasDepA && !hasDepB);
}

bool ArithmeticLogicUnit::isBusy()
{
	return (curStation != nullptr);
}

bool ArithmeticLogicUnit::hasOutput()
{
	return (curCycle == numCycles);
}

unsigned int ArithmeticLogicUnit :: getOutput (Register& instruction, Register& a, Register& b)
{
	if (curCycle != numCycles)
		return 0;

	Instruction inst = Instruction (instruction.getValue ());
	switch (inst.op)
	{
		case fld:
		{
			reg_type address = calculateAddress (a, inst);
			return DataCache :: loadFloatingPoint (address);
		}
		case addi:
		{
			return addImmediate (a, inst);
		}
		case fsd:
		{
			return a.getValue () + inst.imm12;
		}
		case opcode::add:
		{
			return add (a, b);
		}
		case fadd:
		{
			return addFloat (a, b);
		}
		case fsub:
		{
			return subFloat (a, b);
		}
		case fmul:
		{
			return multFloat (a, b);
		}
		case fdiv:
		{
			return divFloat (a, b);
		}
		case bne:
		{
			return branchNotEqual (a, b, inst);
		}
	}
	return 0xFFFFFFFF;
}

unsigned int ArithmeticLogicUnit :: calculateAddress (Register& reg, Instruction& inst)
{
	unsigned int immediate = inst.imm12;
	unsigned int base_address = reg.getValue ();
	return base_address + immediate;
}

unsigned int ArithmeticLogicUnit :: addImmediate (Register& reg, Instruction& inst)
{
	return reg.getValue () + inst.imm12;
}

unsigned int ArithmeticLogicUnit :: add (Register& reg1, Register& reg2)
{
	return reg1.getValue () + reg2.getValue ();
}

unsigned int ArithmeticLogicUnit :: addFloat (Register& reg1, Register& reg2)
{
	float a = (float) reg1.getValue ();
	float b = (float) reg2.getValue ();
	return (unsigned int) (a + b);
}

unsigned int ArithmeticLogicUnit :: subFloat (Register& reg1, Register& reg2)
{
	float a = (float) reg1.getValue ();
	float b = (float) reg2.getValue ();
	return (unsigned int) (a - b);
}

unsigned int ArithmeticLogicUnit :: multFloat (Register& reg1, Register& reg2)
{
	float a = (float) reg1.getValue ();
	float b = (float) reg2.getValue ();
	return (unsigned int) (a * b);
}

unsigned int ArithmeticLogicUnit :: divFloat (Register& reg1, Register& reg2)
{
	float a = (float) reg1.getValue ();
	float b = (float) reg2.getValue ();
	return (unsigned int) (a / b);
}

unsigned int ArithmeticLogicUnit :: branchNotEqual (Register& reg1, Register& reg2, Instruction& inst)
{
	if (reg1.getValue () == reg2.getValue ())
	{
		return inst.imm12;
	} else {
		return 0;
	}
}