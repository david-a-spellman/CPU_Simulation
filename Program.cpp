#include "Program.h"
#include "Instruction.h"

Program::Program(std::string& c)
{
	prog_code = assemble(c);
}

Program::Program(std::ifstream& file)
{
	file.clear();
	file.seekg(0, file.end);
	std::streamoff n = file.tellg();
	file.seekg(0, file.beg);
	char* buffer = new char[(unsigned int)n];
	file.read(buffer, n);
	std::string c(buffer);
	prog_code = assemble(c);
	delete[] buffer;
}

std::vector<reg_type>& Program::getMachineCode()
{
	return prog_code;
}

std::vector<reg_type> Program::assemble(std::string& prog)
{
	std::vector<reg_type> machine_code;
	std::stringstream ss(prog);
	const int max_line = 64;
	char line[max_line];
	std::map<std::string, unsigned int> labelmap;
	int address = 0;

	// skip to the code
	bool found_blank = false;
	while (!found_blank)
	{
		ss.getline(line, max_line);
		std::string s(line);
		if (s.empty())
			break;
	}

	while (!ss.eof())
	{
		ss.getline(line, max_line);
		std::string op(line);
		if (op.empty())
			break;
		Instruction ins(op, labelmap);
		if (!ins.branch_label.empty())
			labelmap[ins.branch_label] = address;
		machine_code.push_back(ins.code);
		address += 4;
	}
	return machine_code;
}
