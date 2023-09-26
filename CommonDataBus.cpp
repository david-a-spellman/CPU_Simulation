#include "CommonDataBus.h"
#include <map>
#include "CircularROB.h"
#include "RegisterFile.h"
#include "DataCache.h"
#include "ArithmeticLogicUnit.h"
using namespace riscv_sim;
using namespace std;

CommonDataBus :: CommonDataBus (int nw, int nb) : NW (nw), NB (nb) {}

void CommonDataBus::addALU (ArithmeticLogicUnit* alu, std::string key)
{
	alu_map [key] = alu;
}

void CommonDataBus::addROB (CircularROB* rb)
{
	rob = rb;
}

void CommonDataBus::addRF (RegisterFile* regf)
{
	rf = regf;
}

void CommonDataBus::addDC (DataCache* datc)
{
	dc = datc;
}

void CommonDataBus::issue (string key, unsigned int index, unsigned int inst_ID, unsigned int instruction, std::vector<reg_type>& data_deps)
{
	ArithmeticLogicUnit* alu = alu_map [key];
	alu -> issue (index, inst_ID, instruction, data_deps);
}

void CommonDataBus::sendResult(reg_type inst_id, reg_type value)
{
	std::map <string, ArithmeticLogicUnit*> ::iterator alus = alu_map.begin();
	for (; alus != alu_map.end(); alus++)
	{
		ArithmeticLogicUnit* alu = alus->second;
		std::vector <ReservationStation>& rs = alu->getStations();
		int j = 0;
		for (; j < (int)rs.size(); j++)
		{
			if (rs[j].occupied == true)
			{
				if (rs.at(j).isDependantA && inst_id == rs.at(j).A.getValue())
				{
					rs.at(j).A.set (value);
					rs.at(j).isDependantA = false;
				}
				if (rs.at(j).isDependantB && inst_id == rs.at(j).B.getValue())
				{
					rs.at(j).B.set(value);
					rs.at(j).isDependantB = false;
				}
				if (rs.at(j).isReady == false &&
					rs.at(j).isDependantA == false &&
					rs.at(j).isDependantB == false)
				{
					rs.at(j).isReady = true;
				}
			}
		}
	}
}
