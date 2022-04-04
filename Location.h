#pragma once
#ifndef RAINDATA_H
#define RAINDATA_H
#include <iostream>
#include <iomanip>
#include <string>
#include "RainArray.h"
#include "OutputSettings.h"
//#include "Sorting.h"

// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

// Location objects represent a unique geographical location 
// where rain data has been recorded
struct RainDataEntry {
	int month = 0;
	int year = 0;
	double rain = 0; // monthly rainfall in inches in a given location
	bool isAfter(RainDataEntry entry)
	{ // checks if obj date is after argument date
		return
			(year < entry.year) ? false :
			(year > entry.year) ? true :
			(month > entry.month) ? true : false;
	}
	bool isBefore(RainDataEntry entry)
	{ // checks if obj date is before argument date
		return
			(year > entry.year) ? false :
			(year < entry.year) ? true :
			(month < entry.month) ? true : false;
	}
};

class Location
{
private:
	// RainDataEntry represents an individual data entry to be stored in location.rainChart
	std::string ID = ""; 
	std::string name = "";
	std::string regionName = "";
	double avgRainByMonth[12] = {0};
	RainArray<RainDataEntry> rainChart; // actual rainfall data
	// linked list of ptrs to data so that RainChart doesn't need to be sorted 
	// to be traversed chronologically (also project requirement)
	//RainList<RainDataEntry*> rainList;  
	//void sortRainList();
	double latitude = 0;
	double longitude = 0;
	friend class LocationArray;
public:
	Location() : ID(""), name(""), regionName("") {}
	void Deallocate() { rainChart.Deallocate(); } // explicitly declared destructor for RainArray obj
	Location(std::string, std::string, std::string);
	Location(std::string, int); // uses single line from govt text file
	Location(std::string); // uses single line from optimized text file
	void setID(std::string str) { this->ID = str; } // sets ID
	void setName(std::string str) { this->name = str; }
	void setRegion(std::string region) { this->regionName = region; }
	std::string getID() const { return ID; }
	std::string getName() const { return name; }
	std::string getRegion() const { return regionName; }
	void printData();  // prints ID, name, region
	void printMoreData(); // calls print data, then prints rain array
	Location operator+(Location); // |!| linear search
	bool operator==(Location foo) const { return (ID == foo.getID()); }
	bool operator!=(Location foo) const { return (ID != foo.getID()); }
	bool hasID() const { return (!ID.empty()); }
	bool hasName() const { return (!name.empty()); }
	bool hasRegion() const { return (!regionName.empty()); }
	void printLocalExtrema() const;
	void appendRainChart(RainDataEntry r) { rainChart.pushback(r); }
	void appendRainChart(std::string entry);
	void removeEntry(RainDataEntry);    //|!| 
	bool hasEntry(RainDataEntry);      // |!| slow linear search
	void replaceEntry(RainDataEntry); //  |!| slow linear search
	int numEntries() const { return rainChart.getSize(); }
	RainDataEntry getEntry(int i) { return rainChart.at(i); }
	std::string outputEntry(int); // like print data, but returns a string 
	void ComputeAvg(); // rather than a std::cout statement
	void MergeRainCharts(Location);
	void setLatLong(double, double);
	void printCoords();
};                               
#endif