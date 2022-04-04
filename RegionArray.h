#pragma once
#ifndef REGIONARRAY_H
#define REGIONARRAY_H
#include "AnyArray.h"
#include "Region.h"

/*Dynamic array of Region objects for use inside LocationArray objects */

class RegionArray : public AnyArray<Region> {
private:
	bool hasRegion(Region);
	int findRegion(std::string);
public:
	void pushback(std::string);
	void pushback(Location*);
	void printEverything() override;
};

#endif // !REGIONARRAY_H