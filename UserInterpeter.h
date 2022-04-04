#pragma once
#ifndef USER_INTERPETER_H
#define USER_INTERPETER_H
#include "LocationArray.h"
#include "GovtInterpeter.h"
#include "RI.h"
#include <fstream>
#include <Windows.h>

// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

// Singleton style user interface class. All user interaction should
// happen through this class

class UI 
{
private:
	const std::string USER_FILE = "UserFile.txt";
	const std::string GOVT_FILE = "GovtFile.txt";
	const std::string BARE_FILE = "JustLocs.txt";
	const std::string COOR_FILE = "Coordinates.txt";
	LocationArray locs;
	RI robot;
	
	UI() : locs(), robot(USER_FILE, GOVT_FILE, BARE_FILE, COOR_FILE, &locs) 
	{/* singleton, should appear only in init */ }

	static UI& init();
	static std::string upperCaseString(std::string);
	static bool isYesOrNo(char c) { return toupper(c) == 'Y' || toupper(c) == 'N'; } // used in userConsent()
	static bool userConsent();
	void searchByRegion();
	void deleteLocation();
	void addLocation();
	void findForEdit();
	void averageRainfall(Location& lo) { lo.ComputeAvg(); }
	void editLocation(Location&);
	void editLocationID(Location&);
	void editRegion(Location&);
	void editName(Location&);
	void pushRainChart(Location&);
	void removeFromRainChart(Location&);
	void compareEntries(Location);
	void locationMenu(Location*);
	void findForEditMultipleResultsLoop(AnyArray<Location*>);
	void printBy();
	void sortSearchDemo();
	void linkedListDemo();
	void fillJustLocs();
	void userLoop();
	std::string userPreexistingRegion();
public:
	static void run();
};
#endif // !USER_INTERPETER_H