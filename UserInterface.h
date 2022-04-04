#pragma once
#define USERINTERFACE_H
#ifndef USERINTERFACE_H
#include "LocationArray.h"
#include "OutputSettings.h"
#include "GovtInterpeter.h"
#include "UserInterpeter.h"
#include <string>
#include <fstream>


// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

// this is file is written without a .cpp because adding a .cpp file caused consistent linker
// errors. While a better solution is probable, writing in-line took less time than
// trying to debug the linker error did

bool isYesOrNo(char c) { return toupper(c) == 'Y' || toupper(c) == 'N'; } // used in userConsent()
bool userConsent() { // asks user for yes or no, accepts Y, N, y, n
	std::string line = "";
	while (!isYesOrNo(line[0])) {
		std::cout << "Please enter Y or N\n";
		std::cin >> line;
	}
	if (toupper(line[0]) == 'N') { return false; }
	else { return true; }
}
void editLocationID(Location& location) {
	std::string user;
	std::cout <<
		"warning! data from government websites will"
		"fail to merge if ID has been changed\n"
		"Enter new ID\n";
	std::cin >> user;
	std::cout << "New ID: " << user << "\nCommit change?\nY. Yes\n";
	if (userConsent()) { location.setID(user); }
	else { std::cout << "Ok, ID will remain " << location.getID() << '\n'; }
}
void editRegion(Location& location) {// used in editLocation()
	std::string user;
	std::cout << "Enter new region:\n";
	std::cin >> user;
	std::cout << "New ID: " << user << "\nCommit change?\nY. Yes\n";
	if (userConsent()) { location.setRegion(user); }
	else { std::cout << "Ok, region will remain " << location.getRegion() << '\n'; }
}
void editName(Location& location) { // used in editLocation()
	std::string user;
	std::cout << "Enter new name:\n";
	std::cin >> user;
	std::cout << "New name: " << user << "\nCommit change?\nY. Yes\n";
	if (userConsent()) { location.setName(user); }
    else { std::cout << "Ok, name will remain " << location.getName() << '\n'; }
}
void pushRainChart(Location& location) {
	std::string user, month, year, rainfall;
	RainDataEntry entry;
	while (true) {
		std::cout << "Enter entry month as integer (1 - 12)\n";
		std::cin >> month;
		std::cout << "Enter year as four digit integer\n";
		std::cin >> year;
		std::cout << "Enter rainfall in inches a decimal\n";
		std::cin >> rainfall;
		try {
			entry.month = stoi(month);
			entry.year = stoi(year);
			entry.rain = stod(rainfall);
		}
		catch (...) {
			std::cout <<
				"One of your variables could not be read correctly!\n"
				"Please try again using only numbers (no spaces or letters)\n"
				"and press enter after each variable is typed\n";
			std::cout << "Try again?\n";
			if (!userConsent()) { return; }
			continue;
		}
		std::cout
			<< "Your entry states that in "
			<< location.getName() << ", " << entry.rain
			<< " inches of rainfall occurred during month "
			<< entry.month << ", " << entry.year << '\n'
			<< "Is this correct?\n";

		if (!userConsent()) {
			std::cout << "Try again?\n";
			if (!userConsent()) { return; }
			else { continue; }
		}
		if (location.hasEntry(entry)) {
			std::cout
				<< "Entry for month and year already exists!\n"
				<< "Overwrite previous entry?\n";
			if (!userConsent()) { break; }
			location.removeEntry(entry);
		}
		location.appendRainChart(entry);
		std::cout << "Entry replaced\n";
		std::cout << "\nEnter more rainfall data?\n";
		if (!userConsent()) { return; }
	}
}
void removeFromRainChart(Location& location) {
	while (true) {
		std::string month, year;
		RainDataEntry entry;
		std::cout <<
			"Enter month of entry to be deleted"
			" (integer from 1 - 12)\n";
		std::cin >> month;
		std::cout << "Enter year of entry to be deleted\n";
		std::cin >> year;
		try {
			entry.month = stoi(month);
			entry.year = stoi(year);
		}
		catch (...) {
			std::cout <<"Invalid entry, please use integers only\n";
			std::cout << "Try again?\n";
			if (userConsent()) { continue; }
			else { return; }
		}
		if (location.hasEntry(entry)) {
			location.removeEntry(entry);
			std::cout << "Entry removed\n";
		}
		else { std::cout << "Entry not found\n"; }
		std::cout << "Delete another entry?\n";
		if (!userConsent()) { return; }
	}
}
void compareEntries(Location location) {
	RainDataEntry entries[2];
	size_t i = 0;
	while (i < 2) {
		std::string month, year;
		std::cout <<
			"Enter month of " << ((i == 0) ? "first" : "second") << " entry to be compared"
			" (integer from 1 - 12)\n";
		std::cin >> month;
		std::cout << "Enter year of " << ((i == 0) ? "first" : "second") << " entry to be compared\n";
		std::cin >> year;
		try {
			entries[i].month = stoi(month);
			entries[i].year = stoi(year);
		} catch (...) {
			std::cout << "Invalid entry, please use integers only\n";
			std::cout << "Try again?\n";
			if (userConsent()) { continue; }
			else { return; }
		}
		if (location.hasEntry(entries[i])) {
			// add find location index function
			i++;
		}
		else {
			std::cout << "Entry not found\n";
			std::cout << "Try again?\n";
			if (!userConsent()) { return; }
		}
	}
	if (entries[0].rain < 0 || entries[1].rain < 0) {
		std::cout << "One or both of the entries you selected are missing rain data. Try again?\n";
		if (userConsent()) { compareEntries(location); }
		return;
	}
	double difference = entries[0].rain - entries[1].rain;
	std::cout << entries[0].rain << ' ' << entries[1].rain << '\n';
	if (difference == 0) { std::cout << "The entries you selected had equal precipitation!\n"; }
	else if (difference < 0) {
		std::cout << "The month of " << entries[1].month << '/' << entries[1].year <<
			" had " << abs(difference) << " more inches of precipitation than"
			"the month of " << entries[0].month << '/' << entries[0].year << '\n';
	}
	else if (difference > 0) {
		std::cout << "The month of " << entries[0].month << '/' << entries[0].year <<
			" had " << abs(difference) << " more inches of precipitation than"
			"the month of " << entries[1].month << '/' << entries[1].year << '\n';
	}
}
void editLocation(Location& location) { // changes location data based on user input
	std::string user; // user input
	std::string foo; // more user input
	std::cout <<
		"\nA. Change ID\nB. Change name\nC. Change region\n"
		"D. Enter rain data\nE. Delete rain data\nF. Compare two entries\n";
		"Q. Quit\n";
	std::cin >> user;
	switch (toupper(user[0])) 
	{
	case 'A':
		editLocationID(location);
		break;
	case 'B':
		editName(location);
		break;
	case 'C':
		editRegion(location);
		break;
	case 'D':
		pushRainChart(location);
		break;
	case 'E':
		removeFromRainChart(location);
		break;
	case 'F':
		compareEntries(location);
		break;
	case 'Q': return;
	}
}
std::string upperCaseString(std::string str) {
	std::string capitalStr = "";
	for (int i = 0; i < str.size(); i++) {
		capitalStr += char(toupper(str[i]));
	}
	return capitalStr;
}
void averageRainfall(Location& lo) {
	lo.ComputeAvg();
}
void locationMenu(
	Location* loc, 
	LocationArray& locations, 
	void(*find)(LocationArray&) // function pointer prevents link error
) {
	std::cout << '\n';
	loc->printData();
	while (true) {
		std::cout <<
			"\nA. View rainfall data for this location\nB. View averages for this location \n"
			"C. Edit data for this location \nD. Find another location\nQ. Quit\n";
		std::string user = "";
		std::cin >> user;
		switch (toupper(user[0])) {
		case 'A':
			loc->printMoreData();
			break;
		case 'B':
			averageRainfall(*loc);
			break;
		case 'C':
			editLocation(*loc);
			break;
		case'D':
			find(locations);
			return;
		case 'Q':
			return;
		}
	}
}
void findForEditMultipleResultsLoop(
	AnyArray<Location*> locs, 
	LocationArray& locations, 
	void(*find)(LocationArray&) // function pointer prevents link error
) {
	std::string user;
	std::cout << locs.getSize() << " locations found\n";
	for (size_t i = 0; i < locs.getSize(); i++) {
		std::cout << i << ": ";
		locs[i]->printData();
	}
	while (true) {
		std::cout <<
			"Please select a location "
			"by entering the preceding integer\n";
		std::cin >> user;
		try {
			int index = stoi(user);
			if (index >= 0 && index < locs.getSize()) {
				locationMenu(locs[index], locations, find);
				return;
			}
			else {
				std::cout << "Index out of range!\nTry again?";
				if (userConsent()) { continue; }
				else { return; }
			}
		}
		catch (...) { std::cout << "Invalid integer!\n"; }
		std::cout << "Try again?\n";
		if (userConsent()) { continue; }
		else { return; }
	}
}
//void findForEdit(LocationArray& locations) {
//	std::string user;
//	std::cout << "\nType location ID\n";
//	std::cin >> user;
//	user = upperCaseString(user);
//	AnyArray<Location*> locs = locations.searchByID(user);
//	switch (locs.getSize()) {
//	case 0:
//		std::cout << "No locations found!\nTry again?\n";
//		if (userConsent()) { findForEdit(locations); }
//		return;
//	case 1:
//		std::cout << "1 location found\n";
//		locationMenu(locs[0], locations, findForEdit);
//		return;
//	default:
//		findForEditMultipleResultsLoop(locs, locations, findForEdit);
//	}
//}
//void addLocation(LocationArray& locations) {
//	while (true) {
//		Location location;
//		std::string user;
//		std::cout << 
//			"Single word entries only, " 
//			"use underscores in place of spaces if needed\n";
//		std::cout << "Enter location's ID\n";
//		std::cin >> user;
//		location.setID(user);
//		std::cout << "Enter Location's name\n";
//		std::cin >> user;
//		location.setName(user);
//		std::cout << "Enter location's region\n";
//		std::cin >> user;
//		location.setRegion(user);
//		location.printData();
//		std::cout << "Add this location?\n";
//
//		if (userConsent()) { 
//			for (int i = 0; i < locations.size(); i++) {
//				if (locations.at(i) == location) {
//					std::cout <<
//						"Location ID already exists"
//						" Override this location?\n";
//					if (userConsent()) {
//						std::cout << "Okay\n";
//						locations.set(location, i);
//					}
//					std::cout << "Add another location?\n";
//					if (!userConsent()) { return; }
//					else { continue; }
//				}
//			}
//			locations.pushback(location);
//			std::cout << "Location added\n";
//		}
//		std::cout << "Add another location?\n";
//		if (!userConsent()) { return; }
//	}
//}
//void deleteLocation(LocationArray& locations) {
//	bool found = false;
//	while (true) {
//		std::string user;
//		std::cout << "Enter ID of location you wish to delete\n";
//		std::cin >> user;
//		for (int i = 0; i < locations.size(); i++) {
//			if (locations.at(i).getID() == user) {
//				found = true;
//				locations.at(i).printData();
//				std::cout << "Delete this location?\n";
//				if (userConsent()) {
//					locations.remove(i);
//				}
//			}
//		}
//		if (!found) { std::cout << "Location not found!\n"; }
//		std::cout << "Continue removing items?\n";
//		if (!userConsent()) { return; }
//	}
//}
//void inFile(LocationArray& locations, std::string file) { // in files data from optimized text
//	std::fstream infile(file, std::ios::in);
//	std::string line, entry;
//	Location location;
//	if (!infile) {
//		std::cout << file << " couldn't be opened!\n";
//		return;
//	}
//	while (getline(infile, line)) {
//		if (line.empty()) { continue; } // extra lines should be eliminated?
//		location = Location(line);
//		while (infile >> entry) {
//			if (entry[0] == '!') { break; }
//			location.appendRainChart(entry);
//		}
//		locations.pushback(location);
//	}
//	infile.close();
//}
//void outFile(LocationArray& locations, std::string file) { // out files data in optimized format
//	std::fstream outfile(file, std::ios::out);
//	std::string line = "";
//	int i, j, size = locations.size();
//	const int ENTRIES_PER_LINE = 6;
//	for (i = 0; i < size; i++) {
//		line += locations.at(i).getID()
//			+ " " + locations.at(i).getName() +
//			" " + locations.at(i).getRegion() + '\n';
//		for (j = 0; j < locations.at(i).numEntries(); j++) {
//			line += locations.at(i).outputEntry(j);
//			if (j == locations.at(i).numEntries() - 1) { line += " !"; }
//			if (!((j + 1) % ENTRIES_PER_LINE)) { line += '\n'; }
//		}
//		if (locations.at(i).numEntries() == 0) { line += " !"; }
//		outfile << line << "\n\n"; // '!' marks end of data for location
//		line = "";
//	}
//	outfile.close();
//}
//void searchByRegion(LocationArray& locs) {
//	std::string user;
//	Region* reg;
//	int index;
//	std::cout << "Do you know you're regions index?\n";
//	if (!userConsent()) { locs.printRegions(); }
//	while (true) {
//		std::cout << "Enter your regions index (the integer preceding region)\n";
//		std::cin >> user;
//		try { 
//			index = stoi(user); 
//			reg = locs.atRegionReference(index);
//			break;
//		}
//		catch (...) {
//			std::cout << "Invalid index!\nTry again?\n";
//			if (userConsent()) { continue; }
//			else { return; }
//		}
//	}
//	std::cout <<
//		reg->getName() << 
//		"\nSize: " << reg->getSize() <<
//		"\nSee locations under this region?\n";
//	if (userConsent()) { reg->printLocations(); }
//}
void userLoop(std::string userFile, std::string govtFile) {
	std::string user;
	LocationArray locations;
	bool done = false;
	std::cout <<
		"Welcome to RainChart!\n\nPlease choose a data source "
		"by entering the preceding letter\n"
		"A: " << userFile << "\nB: " << govtFile << '\n';
	while (!done) {
		getline(std::cin, user);
		switch (toupper(user[0])) {
		case 'A':
			UserInterpeter::interpetUserData(locations, userFile);
			done = true;
			break;
		case 'B':
			govtFileInterpeter::interpetGovtData(locations, govtFile);
			done = true;
			break;
		default:
			std::cout << "Please choose A or B!\n";
		}
	}
	locations.sortLocations();
	int entries = locations.numEntries();
	std::cout
		<< entries << " total data entries found from " 
		<< locations.size() << " unique locations.\n";
	if (!entries) {
		std::cout << 
			"\nPlease check the file contains valid data "
			"and that formatting is compatible with the function "
			"being used to read file\n";
		return;
	}
	while (true) {
		std::cout <<
			"\nTo view actual rainfall choose option B and "
			"enter ID for the location you are investigating\n"
			"If you do not know your location's ID, select A to"
			" view all locations, or select R to view regions\n\n"
			"A. Show Locations\n"
			"B. Search by ID (edit/view rainfall)\n"
			"C. Add Location\n"
			"D. Delete location\n"
			"R. Show regions\n"
			"S. Search by region index\n"
			"E. Commit changes and continue\n"
			"F. Commit changes and quit\n"
			"G. Quit without committing changes\n"
			"Type letter preceding choice\n";
		std::cin >> user;

		switch (toupper(user[0]))
		{
		case 'A': // prints in order of in file
			for (int i = locations.size() - 1; i >= 0; i--) {
				locations.at(i).printData(); 
				ScreenBreak(i);
			}
			break;
		case 'B':
			UserInterpeter::findForEdit(locations);
			break;
		case 'C':
			UserInterpeter::addLocation(locations);
			break;
		case 'D':
			UserInterpeter::deleteLocation(locations);
			break;
		case 'R':
			locations.printRegions();
			break;
		case 'S':
			UserInterpeter::searchByRegion(locations);
			break;
		case 'I':
			locations.printByID();
			break;
		case 'E':
			std::cout << "Out filing changes...\n";
			UserInterpeter::outFile(locations, userFile);
			break;
		case 'F':
			std::cout << "Out filing changes...\n";
			UserInterpeter::outFile(locations, userFile);
			return;
		case 'G':
			std::cout << "\nUnsaved changes will be lost. Are you sure?\n";
			if (userConsent()) { return; }
			break;
		default: std::cout << "\nInvalid selection. Select an item by entering the letter preceding it\n";
		}
	}
}
#endif 