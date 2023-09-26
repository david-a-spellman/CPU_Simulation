#include <vector>
#include <sstream>
#include "DataCache.h"
using namespace riscv_sim;

std::vector <unsigned int> DataCache :: memory;

void DataCache :: initialize ()
{
	memory = std :: vector<unsigned int>(10000, 0);
}

void DataCache :: initialize (std :: vector <unsigned int>& vals)
{
	memory = vals;
}

void DataCache::load(std::ifstream& file)
{
	DataCache::initialize();

	using namespace std;
	file.seekg(0, file.end);
	std::streamoff n = file.tellg();
	file.seekg(0, file.beg);

	char* buffer = new char[(unsigned int)n];
	file.read(buffer, n);
	std::string d(buffer);
	std::stringstream ss(d);
	char line[256];
	do {
		ss.getline(line, 256);
	} while (line[0] == '%');
	
	string ln_data;
	int pos = 0;
	do {
		ln_data = line;
		if (ln_data.empty())
			break;
		stringstream ss2(ln_data);
		unsigned int address = 0;
		ss2 >> address;
		while (ss2.peek() == ',' || ss2.peek() == ' ')
			ss2.get();
		unsigned int data = 0;
		ss2 >> data;
		memory.at(address / 4) = data;
		ss.getline(line, 256);
	} while (!ss.eof() && !ln_data.empty());

	delete[] buffer;
}

unsigned int DataCache :: loadFloatingPoint (unsigned int address)
{
	return memory [address / 4];
}

void DataCache :: storeFloatingPoint (unsigned int address, unsigned int value)
{
	memory [address / 4] = value;
}

// EOF