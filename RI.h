#pragma once
#ifndef RI_H
#define RI_H
#include "LocationArray.h"
#include <fstream>

// Stands for robot interface. Autonomous functionality
// for UI (user interface) class. Most of these functions were
// previously in UI, but that class had become difficult to maintain due
// to its large size, and since these functions do not use user input,
// they seemed misplaced in UI

class RI 
{
private:
	std::string user_file, govt_file, bare_file, coor_file;
	LocationArray* locs;
public:
	RI() {}
	RI(std::string, std::string, std::string, std::string, LocationArray*);
	void cleanCoordFile();
	void addCoords();
	bool isValidCoord(std::string);
	void interpetUserData();
	bool isValidCoordUnitTest();
	static bool runAllTests();
	void outFile();
};

#endif // !RI_H

