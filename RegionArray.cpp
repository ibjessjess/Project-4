#include "RegionArray.h"

// prints all the names
void RegionArray::printEverything()
{
	for (size_t i = 0; i < size; i++)
	{
		std::cout << arr[i].getName() << '\n';
	}
}
// appends to end of array
void RegionArray::pushback(std::string str)
{
	if (str.empty()) { return; }
	Region reg(str); // creates region from str
	AnyArray::pushback(reg); // normal push back
}
// overrides AnyArray::pushback()
void RegionArray::pushback(Location* loc)
{ 
	if (loc->getRegion().empty()) { return; } // skip if no region
	int i = findRegion(loc->getRegion()); // find the region
	if (i >= 0) { atReference(i)->pushback(loc); } else
	{
		pushback(loc->getRegion());
		atReference(size - 1)->pushback(loc);
	}
}
// checks if region already exists
bool RegionArray::hasRegion(Region reg)
{ 
	for (size_t i = 0; i < size; i++)
	{
		if (reg == arr[i]) { return true; }
	}
	return false;
}
// finds region index from string
int RegionArray::findRegion(std::string reg)
{ 
	for (int i = 0; i < size; i++)
	{
		if (reg == at(i).getName()) { return i; }
	}
	return -1;
}