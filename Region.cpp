#include "Region.h"

// partially inherited constructor
Region::Region() : AnyArray()
{
	ID = -1;
	name = "";
}
// partially inherited constructor
Region::Region(std::string str) : AnyArray()
{
	ID = -1;
	name = str;
}