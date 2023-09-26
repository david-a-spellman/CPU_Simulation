#ifndef _CircularROB_
#define _CircularROB_
#include "UnsignedIntMath.h"
#include "CommonDataBus.h"
#include <vector>
#include <iostream>
using namespace uim;

namespace riscv_sim
{
	class CommonDataBus;

	class CircularROB
	{
	private:
		unsigned char size;
		std::vector<unsigned int> values;
		std::vector<unsigned int> destinations;
		std::vector<unsigned int> instructions;
		std::vector<bool> ready_bits;
		std::vector<bool> dependant_bits;
		char head;
		char tail;
		CommonDataBus* cdb;
	public:
		CircularROB (unsigned char s, CommonDataBus* bus);
		/** i = instruction, d = destination reg, v = value/result, b = ready flag */
		void set (unsigned char index = 0, unsigned int i = 0, unsigned int d = 0, unsigned int v = 0, bool b = false, bool dep = false);
		unsigned int getInstruction (unsigned char index = 0);
		unsigned int getDestination (unsigned char index = 0);
		unsigned int getValue (unsigned char index = 0);
		bool getReady (unsigned char index = 0);
		bool getDependant(unsigned char index = 0);
		unsigned char getHead ();
		void advanceHead ();
		unsigned char getTail ();
		void advanceTail ();
		bool hasFreeEntry ();
		bool commitHead ();
		bool addEntry (unsigned int i = 0, unsigned int d = 0, unsigned int v = 0, bool b = false, bool dep = false);
		unsigned char getSize ();
		void setFrom (CircularROB& rob);
		friend std::ostream & operator << (std::ostream &out, const CircularROB &r);
		void flush();
		void cycle();
		void write_back(int index);
	};
}
#endif
// EOF