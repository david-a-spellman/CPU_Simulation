#ifndef _COMPONENT_
#define _COMPONENT_
#include "Architecture.h"
#include <vector>
#include <string>


class Component
{
protected:
	std::string name;
	bool isStalled;
	bool writeable;
public:
	Component(std::string n) : name(n), isStalled(false), writeable(true) {}
	virtual void cycle() {}
	virtual reg_type getOutput(int i = 0) const { return 0; }
	virtual size_t numOutputs() const { return 1; }
	std::string getName() const { return name; }
	void setIsStalled(bool stall) { isStalled = stall; }
	void setIsWritable(bool allowed) { writeable = allowed; }
};

#endif
// EOF