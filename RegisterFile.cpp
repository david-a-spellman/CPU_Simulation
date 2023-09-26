#include "RegisterFile.h"
#include "Instruction.h"
namespace riscv_sim {



	RegisterFile::RegisterFile(size_t size) : Component("RegFile")
	{
		char s[32];
		for (size_t i = 0; i < size; ++i)
		{
			sprintf_s(s, "R%d", i);
			reg_file.push_back(Register(s));
		}
		// Make reg 0 read-only
		reg_file.at(0).setIsWritable(false);
	}

	void RegisterFile::cycle()
	{
		// Nothing to do?
	}

	void RegisterFile::pop_with_test_data()
	{
		for (size_t i = 0; i < reg_file.size(); ++i)
		{
			reg_file.at(i).set(i);
		}
	}
	//Register& RegisterFile::getRegA()
	//{
	//	int i = 0;
	//	Instruction fields(input());
	//	i = fields.rs1;
	//	return reg_file.at(i);
	//}
	//Register& RegisterFile::getRegB()
	//{
	//	int i = 0;
	//	Instruction fields(input());
	//	i = fields.rs2;
	//	return reg_file.at(i);
	//}
	Register& RegisterFile::getReg(int num)
	{
		return reg_file.at(num);
	}

	std::ostream & operator << (std::ostream &out, const RegisterFile &r)
	{
		//char str[64];
		//sprintf_s(str, "0x%0X", r.getOutput());
		out << "RegisterFile contents: " << std::endl;
		return out;
	}

}
// EOF