#ifndef _Register_
#define _Register_
#include "UnsignedIntMath.h"
#include "Architecture.h"
#include "Component.h"
#include <iostream>
using namespace uim;

namespace riscv_sim
{
	class Register : public Component
	{
	private:
		static const reg_type max_register_value = 4294967295;
		unsigned int register_value = 0;
		Component* input;
	public:
		Register (std::string name, reg_type val = 0);
		void cycle();
		reg_type getOutput(int i = 0) const;
		reg_type getValue (unsigned int mask = max_register_value, unsigned int shift_amount = 0);
		void set (reg_type val);
		void setInput(Component& c);
		bool equal (Register& reg);
		bool greater (Register& reg);
		bool less (Register& reg);
		bool greaterOrEqual (Register& reg);
		bool lessOrEqual (Register& reg);
		void setFrom (Register& reg);
		friend std::ostream & operator << (std::ostream &out, const Register &r);
	};
}
#endif
// EOF