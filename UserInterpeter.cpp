#include "UserInterpeter.h"

// returns static reference to singleton object
UI& UI::init() 
{
	static UI single;
	return single;
}
// begin function to get called in main
void UI::run() 
{
	UI foo;
	foo.userLoop();
}
// prints values belonging to region if any exist
void UI::searchByRegion() 
{
	std::string user;
	Region* reg;
	int index;
	std::cout << "Do you know you're regions index?\n";
	if (!userConsent()) { locs.printRegions(); }
	while (true) 
	{
		std::cout << "Enter your regions index (the integer preceding region)\n";
		std::cin >> user;
		try 
		{
			index = stoi(user);
			reg = locs.atRegionReference(index);
			break;
		} catch (...) {
			std::cout << "Invalid index!\nTry again?\n";
			if (userConsent()) { continue; }
			else { return; }
		}
	}
	std::cout <<
		reg->getName() <<
		"\nSize: " << reg->getSize() <<
		"\nSee locations under this region?\n";
	if (userConsent()) { reg->printLocations(); }
}
// asks user for yes or no, accepts Y, N, y, n
bool UI::userConsent()
{ 
	std::string line = "";
	while (!isYesOrNo(line[0])) 
	{
		std::cout << "Please enter Y or N\n";
		std::cin >> line;
	}
	if (toupper(line[0]) == 'N') { return false; }
	else { return true; }
}
// deletes a location at a particular index
void UI::deleteLocation() 
{
	bool found = false;
	while (true) {
		std::string user;
		std::cout << "Enter exact ID of location you wish to delete\n";
		std::cin >> user;
		for (int i = 0; i < user.length(); i++) { user[i] = (char)toupper(user[i]); }
		for (int i = 0; i < locs.size(); i++) {
			if (locs.at(i).getID() == user) {
				found = true;
				locs.at(i).printData();
				std::cout << "Delete this location?\n";
				if (userConsent()) {
					locs.remove(i, true);
				}
			}
		}
		if (!found) { std::cout << "Location not found!\n"; }
		std::cout << "Continue removing items?\n";
		if (!userConsent()) { return; }
	}
}
// allows the user to choose a prexisting region
std::string UI::userPreexistingRegion()
{
	std::string regionname;
	int regionIndex;
	locs.printRegions();
	while (true)
	{
		std::cout << "Enter your regions index (the integer preceding region)\n";
		std::cin >> regionname;
		try
		{
			regionIndex = stoi(regionname);
			regionname = locs.atRegionReference(regionIndex)->getName();
			break;
		}
		catch (...)
		{
			std::cout << "Invalid index!\nTry again?\n";
			if (userConsent()) { continue; } 
			else
			{
				std::cout << "Enter location's region\n";
				getline(std::cin, regionname);
				break;
			}
		}
	}
	return regionname;
}
// adds new location with user input
void UI::addLocation() 
{
	while (true) 
	{
		Location location;
		std::string user;
		std::cout <<
			"Single word entries only, "
			"use underscores in place of spaces if needed\n";
		std::cout << "Enter location's ID\n";
		std::cin >> user;
		location.setID(user);
		std::cout << "Enter Location's name\n";
		std::cin >> user;
		location.setName(user);
		std::cout << "Does your location's region already exist in this database?\n";
		if (userConsent()) { user = userPreexistingRegion(); }
		else
		{
			std::cout << "Enter location's region\n";
			std::cin >> user;
		}
		location.setRegion(user);
		std::cout << "\n\nYour location:\n";
		location.printData();
		std::cout << "\nAdd this location?\n";
		if (userConsent()) 
		{
			for (int i = 0; i < locs.size(); i++) 
			{
				if (locs.at(i) == location) 
				{
					std::cout <<
						"Location ID already exists"
						" Override this location?\n";
					if (userConsent()) {
						std::cout << "Okay\n";
						locs.set(location, i, true);
					}
					std::cout << "Add another location?\n";
					if (!userConsent()) { return; }
					else { continue; }
				}
			}
			locs.pushback(location, true /* sort after */);
			std::cout << "Location added\n";
		}
		std::cout << "Add another location?\n";
		if (!userConsent()) { return; }
	}
}
// calls search function
void UI::findForEdit() 
{
	std::string user;
	std::cout <<
		"\nType location ID, first letter of ID, or an initial substring\n";
	std::cin >> user;
	user = upperCaseString(user);
	AnyArray<Location*> results = locs.searchByID(user);
	switch (results.getSize()) 
	{
	case 0:
		std::cout << "No locations found!\nTry again?\n";
		if (userConsent()) { findForEdit(); }
		return;
	case 1:
		std::cout << "1 location found\n";
		locationMenu(results[0]);
		return;
	default:
		findForEditMultipleResultsLoop(results);
	}
}
// returns upper case version of given string
std::string UI::upperCaseString(std::string str) 
{
	std::string capitalStr = "";
	for (int i = 0; i < str.size(); i++) 
	{
		capitalStr += char(toupper(str[i]));
	}
	return capitalStr;
}
// fills just locations file for use by web scraper
void UI::fillJustLocs() {
	std::cout << "Fill " << BARE_FILE << "?\n";
	if (!userConsent()) { return; }
	std::fstream out(BARE_FILE, std::ios::out);
	Location* loc;
	for (size_t i = 0; i < locs.size(); i++) 
	{
		loc = locs.atReference(i);
		out << 
			loc->getID() << '\t' << 
			loc->getName() << '\t' << 
			loc->getRegion() << '\n';
	}
	out.close();
	std::cout << "Done\n";
}
// called if multiple locations are found
void UI::findForEditMultipleResultsLoop(AnyArray<Location*> searchResults)
{
	std::string user;
	std::cout << searchResults.getSize() << " locations found\n";
	for (size_t i = 0; i < searchResults.getSize(); i++) 
	{
		std::cout << std::right << i << ": ";
		searchResults[i]->printData();
	}
	while (true) {
		std::cout <<
			"Please select a location "
			"by entering the preceding integer\n"
			"Press 'Q' to quit \n";
		std::cin >> user;
		if (user == "Q" || user == "q") { return; }
		else 
		{
			try
			{
				int index = stoi(user);
				if (index >= 0 && index < searchResults.getSize())
				{
					locationMenu(searchResults[index]);
					return;
				}
				else
				{
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
}
// gives a menu of stuff user can so regarding a specific location
void UI::locationMenu(Location* loc) 
{
	std::cout << '\n';
	loc->printData();
	while (true) 
	{
		std::cout <<
			"\nA. View rainfall data for this location\nB. View averages for this location \n"
			"C. Edit data for this location \nD. Find another location\nQ. Quit\n";
		std::string user = "";
		std::cin >> user;
		switch (toupper(user[0])) 
		{
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
			findForEdit();
			return;
		case 'Q':
			return;
		}
	}
}
// changes location data based on user input
void UI::editLocation(Location& location) 
{ 
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
// changes location ID based on user input
void UI::editLocationID(Location& location) 
{
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
// changes location's region based on user input
void UI::editRegion(Location& location) 
{
	std::string user;
	std::cout << "Does your  location's region already exist in database?\n";
	if (userConsent()) { user = userPreexistingRegion(); }
	else
	{
		std::cout << "Enter new region:\n";
		std::cin >> user;
	}
	std::cout << "New region: " << user << "\nCommit change?\nY. Yes\n";
	if (userConsent()) { location.setRegion(user); }
	else { std::cout << "Ok, region will remain " << location.getRegion() << '\n'; }
}
// changes location name based on user input
void UI::editName(Location& location) 
{ 
	std::string user;
	std::cout << "Enter new name:\n";
	std::cin >> user;
	std::cout << "New name: " << user << "\nCommit change?\nY. Yes\n";
	if (userConsent()) { location.setName(user); }
	else { std::cout << "Ok, name will remain " << location.getName() << '\n'; }
}
// appends rain chart based on user input
void UI::pushRainChart(Location& location) 
{
	std::string user, month, year, rainfall;
	RainDataEntry entry;
	while (true) 
	{
		std::cout << "Enter entry month as integer (1 - 12)\n";
		std::cin >> month;
		std::cout << "Enter year as four digit integer\n";
		std::cin >> year;
		std::cout << "Enter rainfall in inches a decimal\n";
		std::cin >> rainfall;
		try 
		{
			entry.month = stoi(month);
			entry.year = stoi(year);
			entry.rain = stod(rainfall);
		}
		catch (...) 
		{
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
		if (!userConsent()) 
		{
			std::cout << "Try again?\n";
			if (!userConsent()) { return; }
			else { continue; }
		}
		if (location.hasEntry(entry))
		{
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
// deleted from rain chart based on user input
void UI::removeFromRainChart(Location& location) 
{
	while (true) 
	{
		std::string month, year;
		RainDataEntry entry;
		std::cout 
			<< "Enter month of entry to be deleted"
			<< " (integer from 1 - 12)\n";
		std::cin >> month;
		std::cout << "Enter year of entry to be deleted\n";
		std::cin >> year;
		try 
		{
			entry.month = stoi(month);
			entry.year = stoi(year);
		} 
		catch (...) 
		{
			std::cout << "Invalid entry, please use integers only\n";
			std::cout << "Try again?\n";
			if (userConsent()) { continue; }
			else { return; }
		}
		if (location.hasEntry(entry)) 
		{
			location.removeEntry(entry);
			std::cout << "Entry removed\n";
		}
		else { std::cout << "Entry not found\n"; }
		std::cout << "Delete another entry?\n";
		if (!userConsent()) { return; }
	}
}
// compares two entries based on user input and rain data
void UI::compareEntries(Location location)
{
	RainDataEntry entries[2];
	size_t i = 0;
	while (i < 2) 
	{
		std::string month, year;
		std::cout 
			<< "Enter month of " << ((i == 0) ? "first" : "second") 
			<< " entry to be compared (integer from 1 - 12)\n";
		std::cin >> month;
		std::cout << "Enter year of " << ((i == 0) ? "first" : "second") << " entry to be compared\n";
		std::cin >> year;
		try 
		{
			entries[i].month = stoi(month);
			entries[i].year = stoi(year);
		}
		catch (...) 
		{
			std::cout << "Invalid entry, please use integers only\n";
			std::cout << "Try again?\n";
			if (userConsent()) { continue; }
			else { return; }
		}
		if (location.hasEntry(entries[i]))
		{
			// add find location index function
			i++;
		}
		else 
		{
			std::cout << "Entry not found\n";
			std::cout << "Try again?\n";
			if (!userConsent()) { return; }
		}
	}
	if (entries[0].rain < 0 || entries[1].rain < 0) 
	{
		std::cout << 
			"One or both of the entries you selected"
			" are missing rain data.Try again ? \n";
		if (userConsent()) { compareEntries(location); }
		return;
	}
	double difference = entries[0].rain - entries[1].rain;
	std::cout << entries[0].rain << ' ' << entries[1].rain << '\n';
	if (difference == 0) { std::cout << "The entries you selected had equal precipitation!\n"; }
	else if (difference < 0)
	{
		std::cout << "The month of " << entries[1].month << '/' << entries[1].year <<
			" had " << abs(difference) << " more inches of precipitation than"
			"the month of " << entries[0].month << '/' << entries[0].year << '\n';
	}
	else if (difference > 0) 
	{
		std::cout << "The month of " << entries[0].month << '/' << entries[0].year <<
			" had " << abs(difference) << " more inches of precipitation than"
			"the month of " << entries[1].month << '/' << entries[1].year << '\n';
	}
}
// prints location data one of six orders based on user input
void UI::printBy()
{
	std::string user = "";
	AnyArray<Location*>* sort;
	bool done = false;
	std::cout <<
		"\nPrint by one of the following options:\n"
		"A: ID (alphabetical)\nB: ID (reverse alphabetical)\n"
		"C: Name (alphabetical)\nD: name (reverse alphabetical\n"
		"E: Entries (numerical)\nF: Entries (reverse numerical)\n";


	done = true;
	std::cin.get();
	getline(std::cin, user);
	switch (toupper(user[0]))
	{
	case 'A':
		sort = &locs.AlphaSortID;
		break;
	case 'B':
		sort = &locs.ReverseAlphaSortID;
		break;
	case 'C':
		sort = &locs.AlphaSortName;
		break;
	case 'D':
		sort = &locs.ReverseAlphaSortName;
		break;
	case 'E':
		sort = &locs.NumericSortNumEntries;
		break;
	case 'F':
		sort = &locs.ReverseNumericSortNumEntries;
		break;
	default:
		std::cout << "Invalid input!\nTry again?\n";
		if (userConsent()) { printBy(); }
		return;
	}
	for (size_t i = 0; i < sort->getSize(); i++)
	{
		sort->at(i)->printData();
		ScreenBreak(i);
	}
}
// demos sort functions and time complexity
void UI::sortSearchDemo()
{
	std::cout << "Running sort tests...\n";
	ULONGLONG starttime, endtime;
	struct testcase 
	{
		char mode;
		int n;
	};
	struct result {
		testcase testCase;
		ULONGLONG runtime;
	};
	AnyArray<result> results;
	testcase cases[] = 
	{ 
		{'s', 10}, {'s', 50}, {'s', 100}, {'s', locs.size()},
		{'b', 10}, {'b', 50}, {'b', 100}, {'b', locs.size()},
		{'q', 10 }, {'q', 50} , {'q', 100}, {'q', locs.size()}

	};
	for (testcase casE : cases)
	{
		starttime = GetTickCount64();
		locs.sortLocations(casE.mode, casE.n);
		endtime = GetTickCount64();
		results.pushback({ casE, endtime - starttime });
	}
	std::cout << '\n';
	for (size_t i = 0; i < results.getSize(); i++)
	{
		std::string sorttype =
			(results.at(i).testCase.mode == 's') ? "selection sort" :
			(results.at(i).testCase.mode == 'b') ? "bubble sort" :
			(results.at(i).testCase.mode == 'q') ? "quick sort" :
			(results.at(i).testCase.mode == 'm') ? "merge sort" : "unknown sort";
		std::cout
			<< "Runtime for test case " << sorttype << " with " 
			<< results.at(i).testCase.n << " objects was " 
			<< results.at(i).runtime << "ms\n";
	}
	std::cout << '\n';

	std::cout <<
		"Expected time complexity for selection sort: O(n^2)\n"
		"Expected time complexity for bubble sort: O(n^2)\n"
		"Expected time complexity for quick sort: O(n*log n)\n";
	system("pause");
}
// demos time complexity of linked list algos
void UI::linkedListDemo()
{
	std::cout << "Running linked list tests...\n\n";
	if (locs.size() == 0)
	{
		std::cout << "Can't run tests, no data!\n";
		return;
	}
	// append time complexity tests
	int appendTestSet[] = { 500, 1'000, 5'000, 10'000, 25'000 };
	for (int len : appendTestSet)
	{
		RainList appendtestlist;
		ULONGLONG starttime = GetTickCount64();
		for (size_t i = 0; i < len; i++)
		{
			int index = i % locs.size();
			IDLookup value = { locs.atReference(index)->getID(), index };
			appendtestlist.append(value);
		}
		ULONGLONG endtime = GetTickCount64();
		std::cout
			<< "Time to append " << len << " items: "
			<< (endtime - starttime) << " ms\n";
	}

	// item removal time complexity tests
	int deleteTestSet[] = { 500, 1'000, 2'500, 5'000, 10'000, 25'000 };
	for (int len : deleteTestSet)
	{
		RainList deletetestlist;
		ULONGLONG starttime = GetTickCount64();
		for (size_t i = 0; i < len; i++)
		{
			int index = i % locs.size();
			IDLookup value = { locs.atReference(index)->getID(), index };
			deletetestlist.append(value);
		}
		for (size_t i = 0; i < (len / locs.size()); i++) // removes all instances of first obj
		{
			deletetestlist.remove({ locs.atReference(0)->getID(), 0 });
		}
		ULONGLONG endtime = GetTickCount64();
		std::cout
			<< "Time to delete " << (len / locs.size()) 
			<< " items from list of length " << len << ": "
			<< (endtime - starttime) << " ms\n";
	}
	std::cout <<
		"\nExpected time complexity to add N items to a new list: O(N)\n"
		"Expected time complexity to add 1 item to list of length N: O(1)\n"
		"Expected time complexity to remove 1 item from list of length N: O(N)\n"
		"Expected time complexity to remove N items from list of length N: O(N^2)\n";
	system("pause");
}
// main loop user used to interact with the rest of the program
// everything loops back to here
void UI::userLoop() 
{
	std::string user;
	bool done = false;
	std::cout <<
		"Welcome to RainChart!\n\nPlease choose a data source "
		"by entering the preceding letter\n\n"
		"IMPORTANT: Any changes committed will overwrite " << USER_FILE << 
		", even if " << GOVT_FILE << " was selected.\n" << GOVT_FILE << 
		" is an exact copy of data found at "
		"https://www.cnrfc.noaa.gov/rainfall_data.php and cannot be changed\n\n"
		"A: " << USER_FILE << "(normal use)\nB: " << GOVT_FILE << "(back up)\n";
	while (!done) 
	{
		getline(std::cin, user);
		switch (toupper(user[0])) 
		{
		case 'A':
			std::cout << "Loading from " << USER_FILE << "...\n";
			robot.interpetUserData();
			done = true;
			break;
		case 'B':
			std::cout << "Loading from " << GOVT_FILE << "...\n";
			GovtFileInterpeter::interpetGovtData(locs, GOVT_FILE);
			done = true;
			break;
		default:
			std::cout << "Please choose A or B!\n";
		}
	}
	locs.sortLocations();

	int entries = locs.numEntries();
	std::cout
		<< entries << " total data entries found from "
		<< locs.size() << " unique locations.\n";
	if (!entries) 
	{
		std::cout <<
			"\nPlease check the file contains valid data "
			"and that formatting is compatible with the function "
			"being used to read file\n";
		return;
	}
	while (true) 
	{
		std::cout <<
			"\nTo view actual rainfall choose option B and "
			"enter ID for the location you are investigating\n"
			"If you do not know your location's ID, select A to"
			" view all locations, or select R to view regions\n\n"
			"A. Show locations\n"
			"B. Search by ID (edit/view rainfall)\n"
			"C. Add Location\n"
			"D. Delete location\n"
			"R. Show regions\n"
			"S. Search by region index\n"
			"I. Show locations sorted by ID\n"
			"E. Commit changes and continue (commits to " << USER_FILE << ")\n"
			"F. Commit changes and quit (commits to " << USER_FILE << ")\n"
			"G. Quit without committing changes\n"
			"\nNEW:\nT: demo sort functions and time complexity\n"
			"L: demo linked list functions and time complexity\n\n"
			"Type letter preceding choice\n";
		std::cin >> user;
		switch (toupper(user[0])) 
		{
		case 'A': // prints in order of in file
			for (int i = locs.size() - 1; i >= 0; i--) {
				locs.at(i).printData();
				//locs.at(i).printCoords();
				ScreenBreak(i);
			}
			break;
		case 'B':
			findForEdit();
			break;
		case 'C':
			addLocation();
			break;
		case 'D':
			deleteLocation();
			break;
		case 'E':
			std::cout << "Out filing changes...\n";
			robot.outFile();
			break;
		case 'F':
			std::cout << "Out filing changes...\n";
			robot.outFile();
			return;
		case 'G':
			std::cout << "\nUnsaved changes will be lost. Are you sure?\n";
			if (userConsent()) { return; }
			break;
		case 'H':break;
		case 'I':
			locs.printByID();
			break;
		case 'J':
			fillJustLocs();
			break;
		case 'K':
			printBy();
			break;
		case 'L':
			linkedListDemo();
			break;
		case 'M':break;
		case 'N':break;
		case 'O':break;
		case 'P':break;
		case 'Q':break;
		case 'R':
			locs.printRegions();
			break;
		case 'S':
			searchByRegion();
			break;
		case 'T':
			sortSearchDemo();
			break;
		case 'U':break;
		case 'V':break;
		case 'W':break;
		case 'X':break;
		case 'Y':break;
		case 'Z':break;
		default: std::cout << "\nInvalid selection. Select an item by entering the letter preceding it\n";
		}
	}
}