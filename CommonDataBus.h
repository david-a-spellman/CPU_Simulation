#ifndef _COMMONDATABUS_
#define _COMMONDATABUS_
#include <map>
#include "Architecture.h"
#include "CircularROB.h"
#include "RegisterFile.h"
#include "DataCache.h"
#include "ArithmeticLogicUnit.h"
#include <string>
using namespace uim;

namespace riscv_sim
{
	class ArithmeticLogicUnit;
	class CircularROB;


	class CommonDataBus
	{
	public:
		std :: map <std::string, ArithmeticLogicUnit*> alu_map;
		int NW;
		int NB;
		RegisterFile* rf;
		CircularROB* rob;
		DataCache* dc;
	public:
		CommonDataBus (int nw, int nb);
		void addALU (ArithmeticLogicUnit* alu, std::string key);
		void addROB (CircularROB* rb);
		void addRF (RegisterFile* regf);
		void addDC (DataCache* datc);
		void issue (std::string key, unsigned int index, unsigned int inst_ID, unsigned int instruction, std::vector<reg_type>& data_deps);
		void sendResult(reg_type inst_id, reg_type value);
	};
}

#endif
// EOF