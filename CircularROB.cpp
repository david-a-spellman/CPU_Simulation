#include "Register.h"
#include "CircularROB.h"
namespace riscv_sim {

	CircularROB::CircularROB(unsigned char s, CommonDataBus* bus) :
		values(s, 0),
		destinations(s, 0),
		instructions(s, 0),
		ready_bits(s, false),
		dependant_bits(s, false),
		cdb(bus)
	{
		size = s;
		head = tail = -1;
	}

	void CircularROB::set(unsigned char index, unsigned int i, unsigned int d, unsigned int v, bool b, bool dep) {
		if (index >= size)
		{
			printf("ERROR! bad ROB index.");
		}
		else {
			instructions[index] = i;
			destinations[index] = d;
			values[index] = v;
			ready_bits[index] = b;
			dependant_bits[index] = dep;
		}
	}

	unsigned int CircularROB::getInstruction(unsigned char index)
	{
		if (index >= size)
		{
			return 0;
		}
		else {
			return instructions[index];
		}
	}

	unsigned int CircularROB::getDestination(unsigned char index)
	{
		if (index >= size)
		{
			return 0;
		}
		else {
			return destinations[index];
		}
	}

	unsigned int CircularROB::getValue(unsigned char index)
	{
		if (index >= size)
		{
			return 0;
		}
		else {
			return values[index];
		}
	}

	bool CircularROB::getReady(unsigned char index)
	{
		if (index >= size)
		{
			return 0;
		}
		else {
			return ready_bits[index];
		}
	}

	bool CircularROB::getDependant(unsigned char index)
	{
		if (index >= size)
		{
			return 0;
		}
		else {
			return dependant_bits[index];
		}
	}

	unsigned char CircularROB::getHead()
	{
		return head;
	}

	void CircularROB::advanceHead()
	{
		if (head == (size - 1) || head == -1)
		{
			head = 0;
		}
		else {
			head += 1;
		}
	}

	unsigned char CircularROB::getTail()
	{
		return tail;
	}

	void CircularROB::advanceTail()
	{
		if (tail == (size - 1) || tail == -1)
		{
			tail = 0;
		}
		else {
			tail += 1;
		}
	}

	bool CircularROB::hasFreeEntry()
	{
		if (head == -1 && tail == -1)
		{
			head = tail = 0;
			return true;
		}
		if (tail == head)
		{
			return false;
		}
		else {
			return true;
		}
	}

	bool CircularROB::commitHead()
	{
		if (getReady(getHead()))
		{
			write_back (getHead ()); 
			set(getHead());
			advanceHead();
			return true;
		}
		else {
			return false;
		}
	}

	bool CircularROB::addEntry(unsigned int i, unsigned int d, unsigned int v, bool b, bool dep)
	{
		if (hasFreeEntry())
		{
			set(getTail(), i, d, v, b, dep);
			advanceTail();
			return true;
		}
		else {
			return false;
		}
	}

	unsigned char CircularROB::getSize()
	{
		return size;
	}

	void CircularROB::setFrom(CircularROB& rob)
	{
		size = rob.getSize();
		values = rob.values;
		destinations = rob.destinations;
		instructions = rob.instructions;
		ready_bits = rob.ready_bits;
		dependant_bits = rob.dependant_bits;
		for (unsigned char i = 0; i < size; i++)
		{
			set(i, rob.getInstruction(i), rob.getDestination(i), rob.getValue(i), rob.getReady(i), rob.getDependant(i));
		}
		head = rob.getHead();
		tail = getTail();
	}

	std::ostream & operator << (std::ostream &out, const CircularROB &r)
	{
		//unsigned char head;
		//unsigned char tail;
		out << "CircularROB contents:" << std::endl;
		for (int i = 0; i < r.size; i++)
		{
			out << "   [" << i << "] I: " << outHex(r.instructions.at(i));
			out << " D: " << r.destinations.at(i);
			out << " V: " << r.values.at(i);
			std::string ready((r.ready_bits.at(i) == true) ? "T" : "F");
			out << " R: " << ready.c_str() << std::endl;
		}
		return out;
	}

	void CircularROB::flush()
	{
		head = tail = -1;
		instructions = std::vector <reg_type>(size, 0);
		destinations = std::vector <reg_type>(size, 0);
		values = std::vector <reg_type>(size, 0);
		ready_bits = std::vector <bool>(size, false);
		dependant_bits= std::vector <bool>(size, false);
	}

	void CircularROB::cycle()
	{
		// variable for checking to see how much bandwidth is left on the common data bus for the clock cycle
		// if there is not enough bandwidth then we must wait to commit more instructions during the next clock cycle
		bool can_commit = true;
		unsigned int bandwidth_used = 0;
		while (can_commit && bandwidth_used < cdb->NB)
		{
			can_commit = commitHead();
			bandwidth_used++;
			if (bandwidth_used < cdb->NB && getDependant(getHead()))
			{
				bandwidth_used++;
			}
		}
	}

	// forward the result to the reservation stations and also write back to the register file
	void CircularROB::write_back(int index)
	{
		reg_type result = getValue(index);
		reg_type address = getInstruction(index);
		cdb->sendResult(address, result);
		RegisterFile* reg_file = cdb->rf;
		reg_type dest = getDestination(index);
		reg_file->getReg(dest).set (result);

	}

}