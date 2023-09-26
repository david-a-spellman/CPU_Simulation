#ifndef _RegisterFile_
#define _RegisterFile_
#include "UnsignedIntMath.h"
#include "Architecture.h"
#include "Register.h"
#include <vector>
#include <string>
#include <iostream>
using namespace uim;

namespace riscv_sim
{
	// General registers (GPR File)
	class RegisterFile : public Component
	{
	public:
		std::vector<Register> reg_file;
	public:
		RegisterFile(size_t size);
		void cycle();
		void pop_with_test_data();
		//Register& getRegA();
		//Register& getRegB();
		Register& getReg(int num);
		friend std::ostream & operator << (std::ostream &out, const RegisterFile &r);
	};

}

#endif
//EOF
