#ifndef _ARCHITECTURE_
#define _ARCHITECTURE_
#include <string>
#include <cstdio>

typedef unsigned int reg_type;

// constant opcode fields
static const unsigned char addi_opcode = 0x13;
static const unsigned char add_opcode = 0x33;
static const unsigned char bne_opcode = 0x63;
static const unsigned char load_fp_opcode = 0x07;
static const unsigned char store_fp_opcode = 0x27;
static const unsigned char float_opcode = 0x53;
// constant funct3 fields
static const unsigned char addi_funct3 = 0x00;
static const unsigned char add_funct3 = 0x00;
static const unsigned char bne_funct3 = 0x01;
static const unsigned char store_fp_funct3 = 0x02;
static const unsigned char load_fp_funct3 = 0x02;
// Constant funct7 fields
static const unsigned char fadd_funct5 = 0x00;
static const unsigned char fsub_funct5 = 0x01;
static const unsigned char fmult_funct5 = 0x02;
static const unsigned char fdiv_funct5 = 0x03;

static const unsigned char RM_RNE = 0; // Round to nearest, ties to even
static const unsigned char RM_RTZ = 1; // Round towards zero
static const unsigned char RM_RDN = 2; // Round down (towards -infinity)
static const unsigned char RM_RUP = 3; // Round up (towards +infinity)
static const unsigned char RM_RMM = 4; // Round to nearest, ties to max magnitude

enum format { R, I, S, U, format_error };
enum opcode {
	nop,
	add,
	addi,
	bne,
	fld,
	fsd,
	fadd,
	fsub,
	fmul,
	fdiv,
	label
};

const std::string int_alu = "int_alu";
const std::string float_add_alu = "float_add_alu";
const std::string float_mult_alu = "float_mult_alu";
const std::string float_div_alu = "float_div_alu";
const std::string data_alu = "data_alu";
const std::string branch_alu = "branch_alu";

std::string outHex(reg_type val);

#endif
