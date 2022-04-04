#pragma once
#ifndef LOCATIONARRAY_H
#define LOCATIONARRAY_H
#include "Location.h"
#include "RegionArray.h"
#include "Sorting.h"
#include "RainHash.h"
#include "OutputSettings.h"


// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

/*
* DynamicArray for location objects. Could have been inherited from AnyArray,
* but was implemented before AnyArray existed, and at this point would not
* be worth effort of implementing inheritance. For inheritance example, see
* RegionArray.h */

class LocationArray 
{
	// basic dynamic array stuff
	static const int START_CAPACITY = 20;
	static const int GROWTH_RATE = 2;
	Location* arr;
	int maxLocations;
	int numLocations;

	// RainHash and accessories
	int const HAS_LEN = 50;
	RainHash hashMap = HAS_LEN;
	//struct IDLookup 
	//{
	//	std::string ID; // location ID
	//	size_t index; // where it appears in LocationArray
	//};

	// sorted pointer arrays for P3 submission
	AnyArray<Location*> AlphaSortID;
	AnyArray<Location*> ReverseAlphaSortID;
	AnyArray<Location*> AlphaSortName;
	AnyArray<Location*> ReverseAlphaSortName;
	AnyArray<Location*> NumericSortNumEntries;
	AnyArray<Location*> ReverseNumericSortNumEntries;

	// other
	RegionArray regions;
	Location nullLocation = Location();
	void reallocateRegions();
	void clean_location_members(int);
	void sortRainArray(int);
	friend class UI;
public:
	LocationArray();
	void sortLocations(char sortmode = 'q', int stop = -1);
	void printByID();
	void printbyName();
	~LocationArray();
	void add(Location l, int i) { set(l + at(i), i); }
	void add(int i, int j) { add(at(j), i); }
	Location at(int) const;
	Location at(size_t i) const { return at((int)i); }
	Region atRegion(int) const;
	Region atRegion(size_t i) const { return atRegion((int)i); }
	Location* atReference(int) const;
	Location* atReference(size_t i) const { return atReference(int(i)); }
	Region* atRegionReference(int) const;
	Region* atRegionReference(size_t i) const {
		return atRegionReference(int(i));
	}
	AnyArray<Location*> searchByID(std::string);
	int size() const { return numLocations; }
	int numEntries() const;
	int find(Location);
	void printRegions();
	int sizeRegions() const { return regions.getSize(); }
	void pushback(Location, bool isUser = false);
	void remove(int, bool isUser = false);
	void set(Location, int, bool isUser = false);
};
#endif
