#ifndef _INSTRUCTION_
#define _INSTRUCTION_
#include "Architecture.h"
#include <string>
#include <sstream>
#include <map>


class Instruction
{
public:
	format f;
	opcode op;
	unsigned char op_bits;
	unsigned char rd;
	unsigned char rs1;
	unsigned char rs2;
	unsigned char funct3;
	unsigned char funct5;
	unsigned char funct7;
	unsigned int imm12;
	unsigned int imm20;
	reg_type code;
	std::string branch_label;
	std::map <std::string, unsigned int>* labels;
public:
	Instruction(std::string& s, std::map<std::string, unsigned int>& labelmap);
	Instruction(reg_type c);
	Instruction(const Instruction& ins);
	virtual reg_type encode(std::string& s);
	unsigned char getReg(std::stringstream& ss);
	unsigned int getImm(std::stringstream& ss);
	virtual opcode strToOpcode(std::string s);
	std::string opcodeToStr(opcode o) const;
	unsigned char opcodeBits(opcode o) const;
	unsigned char funct3Bits(opcode o) const;
	unsigned char funct7Bits(opcode o) const;
	opcode decodeOp(reg_type code);
	virtual format opcodeToFormat(opcode o);
	virtual reg_type buildVal();
	virtual void decode(reg_type c);
	friend std::ostream & operator << (std::ostream &out, const Instruction &i);
	reg_type getCode() { return code; }
	reg_type renameRd(unsigned int new_reg);
	reg_type renameRs1(unsigned int new_reg);
	reg_type renameRs2(unsigned int new_reg);
};

#endif