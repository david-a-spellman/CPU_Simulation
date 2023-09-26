#ifndef _PROGRAM_
#define _PROGRAM_
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Architecture.h"


// Load program from a file as assembly language, convert to machine
// code and provide access to the block of machine code.
class Program
{
private:
	std::vector<reg_type> prog_code;
public:
	Program(std::string& c);
	Program(std::ifstream& file);
	std::vector<reg_type>& getMachineCode();
	std::vector<reg_type> assemble(std::string& prog);
};


#endif
