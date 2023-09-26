#include "InstructionUnit.h"
#include "Instruction.h"
#include "ArithmeticLogicUnit.h"
#include <list>
#include <map>
using namespace std;

namespace riscv_sim {

	std::vector<std::string> physRegs;
	std::list <string> free_list;
	std::map <string, string> allocated_registers;

	InstructionUnit::InstructionUnit(int nf, int nw, InstructionCache& IM, CommonDataBus& bus) :
		Component("InstructionUnit"),
		NF(nf),
		NW(nw),
		PC("PC"),
		i_cache(&IM),
		nextDecodeIns(0),
		nextDispatchIns(0),
		branchUnit(16),
		branched(false),
		cdb(&bus)
	{
		for (int i = 1; i < 32; ++i)
		{
			char buf[16];
			sprintf_s(buf, "p%d", i);
			physRegs.push_back(buf);
			free_list.push_back(buf);
		}
	}

	void InstructionUnit::cycle()
	{
		// Dispatch current set of decoded instruction
		bool should_stall = !dispatch();
		if (should_stall)
			setIsStalled(true);

		// Decode the current set of fetched instructions
		decode();

		// Fetch next batch of instructions
		fetch();

		// Either directly set the PC, or inc by NF*4
		if (branched)
		{
			PC.set(PC.getOutput() + branchOffset);
		}
		else
		{
			PC.set(PC.getOutput() + NF * 4);
		}
	}

	void InstructionUnit::fetch()
	{
		// Use the PC to read instructions from the IM
		std::vector<reg_type> next_ins = i_cache->read(PC.getOutput(), NF);
		for (int i = 0; i < (int)next_ins.size(); ++i)
		{
			instructionQueue.push_back(next_ins.at(i));
			issued.push_back(false);
		}
	}

	void InstructionUnit::decode()
	{
		if (instructionQueue.size() > 0)
		{
			for (int i = 0; i < NW && nextDecodeIns != instructionQueue.size(); ++i)
			{
				// Decode next instruction
				unsigned int curAddress = nextDecodeIns * 4;
				Instruction ins(instructionQueue.at(nextDecodeIns++));

				if (ins.op == nop)
					continue;

				// Check for branch instructions and process them (this may render remaing instructions obsolete)
				if (ins.op == bne)
				{
					// Call the branch prediction unit
					branched = branchUnit.makePrediction(0/*curAddress*/); // always use 0 to force a single predictor
					if (branched)
					{
						// Cancel remaining instructions and load next batch starting from the target address
						branchOffset = ins.imm12;
						instructionQueue.erase(instructionQueue.begin() + nextDecodeIns, instructionQueue.end());
						break;
					}
				}

				// Apply register renaming
				reg_type reg_renamed = rename_registers(ins);

				// Move decoded instructions to the dispatch queue
				dispatchQueue.push_back(reg_renamed);
			}
		}
	}

	// Return value signals a stall
	bool InstructionUnit::dispatch()
	{
		if (isStalled || dispatchQueue.size() == 0) return true;

		// Dispatch up to NW instructions
		for (int i = 0; i < NW && nextDispatchIns < (int)dispatchQueue.size(); ++i)
		{
			unsigned int curAddress = nextDispatchIns * 4;
			reg_type code = dispatchQueue.at(nextDispatchIns);

			Instruction inst(code);
			string key = "error";
			switch (inst.op)
			{
			case add: key = int_alu; break;
			case addi: key = int_alu; break;
			case bne: key = branch_alu; break;
			case fsd: key = data_alu; break;
			case fld: key = data_alu; break;
			case fadd: key = float_add_alu; break;
			case fsub: key = float_add_alu; break;
			case fmul: key = float_mult_alu; break;
			case fdiv: key = float_div_alu; break;
			}
			// send the instruction to the ROB
			std :: vector <reg_type> data_deps = addToROB(inst, curAddress);

			// Issue the instruction on the CommonDataBus
			ArithmeticLogicUnit* alu = cdb->alu_map[key];
			std::vector <ReservationStation>& rs = alu->getStations();
			int j = 0;
			for (; j < (int)rs.size(); j++)
			{
				if (rs[j].occupied == false)
				{
					break;
				}
			}
			// stall!!!
			if (j == rs.size())
			{
				return false;
			}
			else {
				alu->issue(j, curAddress, code, data_deps);
				issued.at(nextDispatchIns) = true;
				nextDispatchIns++;
			}
		}
		return true;
	}

	reg_type InstructionUnit::rename_registers(Instruction ins)
	{
		char buf[8]; sprintf_s(buf, "R%d", ins.rd);
		ins.renameRd(atoi(rename_register(buf).c_str() + 1));
		switch (ins.f) // Rename as need per instruction format
		{
		case R:
			sprintf_s(buf, "R%d", ins.rs1);
			ins.renameRs1(atoi(rename_register(buf).c_str() + 1));
			sprintf_s(buf, "R%d", ins.rs2);
			ins.renameRs2(atoi(rename_register(buf).c_str() + 1));
			break;
		case I:
			sprintf_s(buf, "R%d", ins.rs1);
			ins.renameRs1(atoi(rename_register(buf).c_str() + 1));
			break;
		case S:
			sprintf_s(buf, "R%d", ins.rs1);
			ins.renameRs1(atoi(rename_register(buf).c_str() + 1));
			sprintf_s(buf, "R%d", ins.rs2);
			ins.renameRs2(atoi(rename_register(buf).c_str() + 1));
			break;
		case U:
			break;
		}
		return ins.code;
	}

	string InstructionUnit::rename_register(string reg)
	{
		if (reg == "R0" || reg == "F0") return reg;
		if (free_list.size() == 0)
		{
			return reg;
		}
		else {
			string rename = free_list.front();
			free_list.pop_front();
			allocated_registers[reg] = rename;
			return rename;
		}
	}

	void InstructionUnit::free_register(string rename)
	{
		if (allocated_registers.find(rename) == allocated_registers.end())
			return;
		string reg = allocated_registers[rename];
		allocated_registers.erase(rename);
		free_list.push_back(reg);
	}

	bool InstructionUnit::allocate_register(string reg)
	{
		using namespace std;
		//list <Register*>::const_iterator i = find(free_list.begin(), free_list.end(), (&reg));
		//if (i != free_list.end()) {
		//	free_list.erase(i);
		//	allocated_registers[reg.getName()] = &reg;
		//	return true;
		//}
		return false;
	}

	//void InstructionUnit::reset()
	//{
	//	for (int i = 0; i < NW; ++i)
	//		PCs.at(i).reset();
	//}

	std::ostream & operator << (std::ostream &out, const InstructionUnit &r)
	{
		out << "InstructionUnit contents: " << std::endl;

		out << "   PC: " << outHex(r.PC.getOutput()) << std::endl;

		out << "   InstructionQueue: ";
		if (r.instructionQueue.size() > 0)
		{
			int start = r.instructionQueue.size() - r.NF;
			for (int i = start; i < (int)r.instructionQueue.size(); i++)
				out << "[" << outHex(r.instructionQueue.at(i)) << "]";
			out << std::endl;
		}
		else out << "empty." << std::endl;

		out << "   DispathQueue: ";
		if (r.dispatchQueue.size() > 0)
		{
			int start = r.dispatchQueue.size() - r.NF;
			for (int i = start; i < (int)r.dispatchQueue.size(); i++)
				out << "[" << outHex(r.dispatchQueue.at(i)) << "]";
			out << std::endl;
		}
		else out << "empty." << std::endl;

		return out;
	}

	std :: vector <reg_type> InstructionUnit::addToROB(Instruction inst, reg_type address)
	{
		std::vector <reg_type> dependancies;
		unsigned char rs1 = inst.rs1;
		int dependancy = hasRawDependancy(rs1);
		if (dependancy != -1)
		{
			dependancies.push_back((reg_type)dependancy);
		}
		if (inst.f == R || inst.f == S)
		{
			unsigned char rs2 = inst.rs2;
			dependancy = hasRawDependancy(rs2);
			if (dependancy != -1)
			{
				dependancies.push_back((reg_type) dependancy);
			}
		}

		cdb->rob->addEntry(address, inst.rd, 0, false);

		return dependancies;
	}

	int InstructionUnit::hasRawDependancy (unsigned char reg)
	{
		int dependancy = -1;
		CircularROB* rob = cdb->rob;
		for (unsigned int i = rob->getHead(); i < rob->getTail(); i++)
		{
			reg_type dest = rob->getDestination(i);
			if (dest == reg)
			{
				dependancy = (int)rob->getInstruction (i);
				break;
			}
		}
		return dependancy;
	}

}
// EOF