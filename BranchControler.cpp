#include "Register.h"
#include "BranchControler.h"
using namespace riscv_sim;

BranchControler :: BranchControler (unsigned char s) :
	addresses (s, 0),
	predictions (s, false)
{
	size = s;
}

bool BranchControler :: predict (unsigned char index)
{
	return predictions [index];
}

void BranchControler :: transition (unsigned int address, bool prediction, unsigned int target_address)
{
	unsigned int hash = address << 24;
	hash = hash >> 28;
	predictions [hash] = prediction;
	addresses [hash] = target_address;
}

bool BranchControler :: makePrediction (unsigned int address)
{
	unsigned int address2 = address << 24;
	address2 = address2 >> 28;
	unsigned char index = (unsigned char) address2;
	return predict (index);
}

unsigned int BranchControler :: getAddress (unsigned int address)
{
	unsigned int address2 = address << 24;
	address2 = address2 >> 28;
	unsigned char index = (unsigned char) address2;
	return addresses [index];
}