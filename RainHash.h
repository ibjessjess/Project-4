#pragma once
#ifndef RAIN_HASH_H
#define RAIN_HASH_H
#include <iostream>
#include "AnyArray.h"
#include "RainList.h"

// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

// hash table for strings, intended use is to allow
// rapid lookup to check if a location ID already exists
// in a LocationArray object. All strings should be unique,
// even if integer member is different. RainHash should not
// accept insertions where a particular string already exists

class RainHash 
{
private:
	//enum State { fresh = 0, inUse, used };
	int size;
	AnyArray<RainList> table;
	friend class RainHashUnitTest;
	//const IDLookup nullVal = { "", -1 }; // -1 is never an index
	//AnyArray<IDLookup> table;
	//AnyArray<State> states;
public:
	RainHash(int);
	int hash(std::string);
	int hash(IDLookup x) { return hash(x.str); }
	int getnum(std::string);
	void insert(IDLookup);
	void display();
	void remove(std::string);
};
class RainHashUnitTest 
{
public:
	static void runTests();
};
#endif // !RAIN_HASH_H
