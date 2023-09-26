#include "Register.h"
namespace riscv_sim {

	Register::Register(std::string name, reg_type val) :
		Component(name),
		register_value(val),
		input(nullptr)
	{
	}

	void  Register::cycle()
	{
		// get input, move to output value
		if (input != nullptr)
			register_value = input->getOutput();
	}

	reg_type Register::getOutput(int) const
	{
		return register_value;
	}

	unsigned int Register::getValue(unsigned int mask, unsigned int shift_amount)
	{
		return ((register_value & mask) >> shift_amount);
	}

	void Register::set(unsigned int val)
	{
		register_value = val;
	}

	void Register::setInput(Component& c)
	{
		input = &c;
	}

	/*
	returns true if the register passed has an equivalent value
	if the object passed as the register parameter is not of type 'Register', the method immediately returns NULL
	*/

	bool Register::equal(Register& reg)
	{
		if (getValue() == reg.getValue())
		{
			return true;
		}
		return false;
	}

	bool Register::greater(Register& reg)
	{
		if (getValue() > reg.getValue())
		{
			return true;
		}
		return false;
	}

	bool Register::less(Register& reg)
	{
		if (getValue() < reg.getValue())
		{
			return true;
		}
		return false;
	}

	bool Register::greaterOrEqual(Register& reg)
	{
		if (greater(reg) || equal(reg))
		{
			return true;
		}
		return false;
	}

	bool Register::lessOrEqual(Register& reg)
	{
		if (less(reg) || equal(reg))
		{
			return true;
		}
		return false;
	}

	void Register::setFrom(Register& reg)
	{
		set(reg.getValue());
	}

	std::ostream & operator << (std::ostream &out, const Register &r)
	{
		char str[64];
		sprintf_s(str, "0x%0X", r.getOutput());
		out << "Register " << r.getName() << " contents: " << str << std::endl;
		return out;
	}

}
// EOF