#include "Instruction.h"

Instruction::Instruction(std::string& s, std::map<std::string, unsigned int>& labelmap) :
	labels(&labelmap)
{
	op_bits = 0;
	rd = rs1 = rs2 = 0;
	funct3 = funct5 = funct7 = 0;
	imm12 = imm20 = 0;
	code = encode(s);
}

Instruction::Instruction(reg_type c)
{
	decode(c);
}

Instruction::Instruction(const Instruction& ins)
{
	f = ins.f;
	op = ins.op;
	op_bits = ins.op_bits;
	rd = ins.rd;
	rs1 = ins.rs1;
	rs2 = ins.rs2;
	funct3 = ins.funct3;
	funct5 = ins.funct5;
	funct7 = ins.funct7;
	imm12 = ins.imm12;
	imm20 = ins.imm20;
	code = ins.code;
	branch_label = ins.branch_label;
	labels = ins.labels;
}

std::string parseToken(std::stringstream& ss)
{
	const int max = 128;
	char buffer[max];
	int pos = 0;
	// skip white space
	while (ss.peek() == ' ' || ss.peek() == '\t' || ss.peek() == ',' || ss.peek() == '(' || ss.peek() == ')')
		ss.get();

	// copy a token
	while (!ss.eof() && ss.peek() != ' ' && ss.peek() != '\t' && ss.peek() != ',' && ss.peek() != '(' && ss.peek() != ')' && ss.peek() != '\n' && pos < max)
		buffer[pos++] = ss.get();

	// skip trailing whitespace
	buffer[pos] = 0;
	while (ss.peek() == ' ' || ss.peek() == '\t' || ss.peek() == ',' || ss.peek() == '(' || ss.peek() == ')')
		ss.get();
	return std::string(buffer);
}

reg_type Instruction::encode(std::string& s)
{
	using namespace std;
	stringstream ss(s);
	std::string lbl;

	// Check for a branch/jump label
	std::string token = parseToken(ss);
	op = strToOpcode(token);
	if (op == label)
	{
		branch_label = token;
		op = strToOpcode(parseToken(ss));
	}
	f = opcodeToFormat(op);

	switch (op)
	{
	case nop: break;
	case add:	//			add		R1, R2, R3
		rd = getReg(ss);
		rs1 = getReg(ss);
		rs2 = getReg(ss);
		funct3 = funct3Bits(op);
		break;
	case addi:	//			addi	R2, R0, 124
		rd = getReg(ss);
		rs1 = getReg(ss);
		imm12 = getImm(ss);
		funct3 = funct3Bits(op);
		break;
	case bne:	//			bne		R1,$0,loop (B-type format)
		rs1 = getReg(ss);
		rs2 = getReg(ss);
		lbl = parseToken(ss);
		imm12 = labels->operator[](lbl);
		break;
	case fld:   // loop:	fld		F0, 0(R1)
		rd = getReg(ss);
		imm12 = getImm(ss);
		rs1 = getReg(ss);
		break;
	case fsd:	//			fsd		F0, 0(R2)
		rs1 = getReg(ss);
		imm12 = getImm(ss);
		rs2 = getReg(ss);
		break;
	case fadd:	//			fadd	F0, F0, F4
	case fsub:	//			fsub	F0, F0, F4
	case fmul:	//			fmul	F0, F1, F2
	case fdiv:	//			fdiv	F1, F2, F3
		rd = getReg(ss);
		rs1 = getReg(ss);
		rs2 = getReg(ss);
		funct7 = funct7Bits(op);
		break;
	}
	op_bits = opcodeBits(op);
	return buildVal();
}

unsigned char Instruction::getReg(std::stringstream& ss)
{
	using namespace std;
	unsigned char reg = 99;
	char r[64];
	while (ss.peek() == ' ') ss.get();
	ss.get(r, 64, ',');
	while (ss.peek() == ',') ss.get();
	if (r[0] == 'R' || r[0] == '$' || r[0] == 'F')
	{
		string num(r + 1);
		reg = (unsigned char)stoi(num);
		while (ss.peek() == ' ') ss.get();
	}
	else printf("ERROR!!! could not parse register!\n");
	return reg;
}

unsigned int Instruction::getImm(std::stringstream& ss)
{
	using namespace std;
	unsigned int imm = 0;
	char v[64];
	ss.get(v, 64, '('); // use '(' as delimiter for following register spec
	ss.get();
	string num(v);
	imm = (unsigned char)stoi(num);
	while (ss.peek() == ' ') ss.get();
	return imm;
}

opcode Instruction::strToOpcode(std::string s)
{
	if (s == "add") return add;
	else if (s == "addi") return addi;
	else if (s == "bne") return bne;
	else if (s == "fld") return fld;
	else if (s == "fsd") return fsd;
	else if (s == "fadd") return fadd;
	else if (s == "fsub") return fsub;
	else if (s == "fmul") return fmul;
	else if (s == "fdiv") return fdiv;
	if (s.back() == ':') return label;
	return nop;
}

std::string Instruction::opcodeToStr(opcode o) const
{
	switch (o)
	{
	case add: return "add";
	case addi: return "addi";
	case bne: return "bne";
	case fld: return "fld";
	case fsd: return "fsd";
	case fadd: return "fadd";
	case fsub: return "fsub";
	case fmul: return "fmul";
	case fdiv: return "fdiv";
	default: return "unknown";
	}
	return "unknown";
}

unsigned char Instruction::opcodeBits(opcode o) const
{
	switch (o)
	{
	case nop: return 0;
	case add: return add_opcode;
	case addi: return addi_opcode;
	case bne: return bne_opcode;
	case fld: return load_fp_opcode;
	case fsd: return store_fp_opcode;
	case fadd:
	case fsub:
	case fmul:
	case fdiv:
		return float_opcode;
	}
	return 0;
}

unsigned char Instruction::funct3Bits(opcode o) const
{
	switch (o)
	{
	case nop: return 0;
	case add: return add_funct3;
	case addi: return addi_funct3;
	case bne: return bne_funct3;
	case fld: return load_fp_funct3;
	case fsd: return store_fp_funct3;
	case fadd:
	case fsub:
	case fmul:
	case fdiv:
		return RM_RNE;
	}
	return 0;
}

unsigned char Instruction::funct7Bits(opcode o) const
{
	switch (o)
	{
	case nop:
	case add:
	case addi:
	case bne:
	case fld:
	case fsd:
		return 0;
	case fadd: return fadd_funct5 * 4;
	case fsub: return fsub_funct5 * 4;
	case fmul: return fmult_funct5 * 4;
	case fdiv: return fdiv_funct5 * 4;
	}
	return 0;
}

format Instruction::opcodeToFormat(opcode o)
{
	switch (o) {
	case add: return R;
	case addi: return I;
	case bne: return S;
	case fld: return I;
	case fsd: return S;
	case fadd: return R;
	case fsub:
	case fmul:
	case fdiv:
		return R;
	}
	return format_error;
}

reg_type Instruction::buildVal()
{
	reg_type val;
	switch (f)
	{
	case R:
		val = funct7; val <<= 5;
		val += rs2; val <<= 5;
		val += rs1; val <<= 3;
		val += funct3; val <<= 5;
		val += rd; val <<= 7;
		break;
	case I:
		val = imm12; val <<= 5;
		val += rs1; val <<= 3;
		val += funct3; val <<= 5;
		val += rd; val <<= 7;
		break;
	case S:
		val = imm12 >> 5; val <<= 5;
		val += rs2; val <<= 5;
		val += rs1; val <<= 3;
		val += funct3; val <<= 5;
		val += imm12 & 0x0000001F; val <<= 7;
		break;
	case U:
		val = imm20; val <<= 5;
		val += rd; val <<= 7;
		break;
	}
	val += op_bits;
	return val;
}

reg_type Instruction::renameRd(unsigned int new_reg)
{
	rd = new_reg;
	code = buildVal();
	return code;
}

reg_type Instruction::renameRs1(unsigned int new_reg)
{
	rs1 = new_reg;
	code = buildVal();
	return code;
}

reg_type Instruction::renameRs2(unsigned int new_reg)
{
	rs2 = new_reg;
	code = buildVal();
	return code;
}

void Instruction::decode(reg_type c)
{
	code = c;
	op_bits = (opcode)(c & 0x0000007F);
	op = decodeOp(code);
	f = opcodeToFormat(op);
	rd = (c & 0x00000F80) >> 7;
	switch (f)
	{
	case R:
		funct3 = (c & 0x00007000) >> 12;
		rs1 = (c & 0x000F8000) >> 15;
		rs2 = (c & 0x01F00000) >> 20;
		funct7 = (c & 0xFE000000) >> 25;
		break;
	case I:
		funct3 = (c & 0x00007000) >> 12;
		rs1 = (c & 0x000F8000) >> 15;
		imm12 = (c & 0xFFF00000) >> 20;
		break;
	case S:
		imm12 = rd;
		funct3 = (c & 0x00007000) >> 12;
		rs1 = (c & 0x000F8000) >> 15;
		rs2 = (c & 0x01F00000) >> 20;
		imm12 += (c & 0xFE000000) >> 25;
		break;
	case U:
		break;
	}
}

opcode Instruction::decodeOp(reg_type /*code*/)
{
	switch (op_bits)
	{
	case addi_opcode: return addi;
	case add_opcode: return add;
	case bne_opcode: return bne;
	case load_fp_opcode: return fld;
	case store_fp_opcode: return fsd;
	case float_opcode:
		switch (funct7 / 4)
		{
		case fadd_funct5: return fadd;
		case  fsub_funct5: return fsub;
		case  fmult_funct5: return fmul;
		case  fdiv_funct5: return fdiv;
		}
		return nop;
	}
	return nop;
}

std::ostream & operator << (std::ostream &out, const Instruction &i)
{
	out << i.opcodeToStr(i.op).c_str() << ": rd = " << (int)i.rd << ", rs1 = " << (int)i.rs1 << ", rs2 = " << (int)i.rs2 << std::endl;
	return out;
}

