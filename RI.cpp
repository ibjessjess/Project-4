#include "RI.h"

// constructor constructs from UI
RI::RI(
	std::string user, 
	std::string govt, 
	std::string bare, 
	std::string coor, 
	LocationArray* locs
) : user_file(user), govt_file(govt), bare_file(bare), coor_file(coor), locs(locs) {}

// interprets optimized text from user file to fill loc array
void RI::interpetUserData()
{
	std::fstream infile(user_file, std::ios::in);
	std::string line, entry;
	Location location;
	if (!infile)
	{
		std::cout << user_file << " couldn't be opened!\n";
		return;
	}
	while (getline(infile, line))
	{
		if (line.empty()) { continue; } // extra lines should be eliminated?
		location = Location(line);
		while (infile >> entry)
		{
			if (entry[0] == '!') { break; }
			location.appendRainChart(entry);
		}
		locs->pushback(location);
	}
	infile.close();
}
// out files data in optimized format
void RI::outFile()
{ 
	std::fstream outfile(user_file, std::ios::out);
	std::string line = "";
	int i, j, size = locs->size();
	const int ENTRIES_PER_LINE = 6;
	for (i = 0; i < size; i++)
	{
		line += locs->at(i).getID()
			+ " " + locs->at(i).getName() +
			" " + locs->at(i).getRegion() + '\n';
		for (j = 0; j < locs->at(i).numEntries(); j++)
		{
			line += locs->at(i).outputEntry(j);
			if (j == locs->at(i).numEntries() - 1) { line += " !"; }
			if (!((j + 1) % ENTRIES_PER_LINE)) { line += '\n'; }
		}
		if (locs->at(i).numEntries() == 0) { line += " !"; }
		outfile << line << "\n\n"; // '!' marks end of data for location
		line = "";
	}
	outfile.close();
}
// adds coordinates from coordinates file to appropriate objects
void RI::addCoords()
{
	int argCount;
	AnyArray<Location*> searchResult;
	std::string ID, latAlpha, longAlpha, latNumeric, longNumeric, line;
	std::fstream file(coor_file, std::ios::in);
	if (!file)
	{
		std::cout << "Couldn't open " << coor_file << '\n';
		return;
	}
	std::cout << "Opened " << coor_file << '\n';

	while (getline(file, line))
	{
		argCount = 0;
		ID = "", latAlpha = "", longAlpha = "", latNumeric = "", longNumeric;
		std::cout << "HI\n";
		for (char c : line)
		{
			if (c == '\t')
			{
				argCount++;
				continue;
			}
			switch (argCount)
			{
			case 0:
				ID += c;
				break;
			case 1:
				latAlpha += c;
				break;
			case 2:
				longAlpha += c;
				break;
			}
		}
		searchResult = locs->searchByID(ID);
		if (searchResult.getSize() == 1)
		{
			searchResult[0]->setLatLong(0, 0);
		}
	}
	file.close();
}
// one time use function for fixing up coor file
void RI::cleanCoordFile()
{
	std::fstream file(coor_file, std::ios::in);
	if (!file) { std::cout << "Couldn't open file!\n"; }
	std::string cleanVersion = "", line, newLine, ID, lat, lonG;
	int argCount;
	while (getline(file, line))
	{
		newLine = "", ID = "", lat = "", lonG = "";
		argCount = 0;
		//check for non-ASCII characters and parse
		for (auto c : line) if (static_cast<unsigned char>(c) <= 127)
		{
			if (c == '\t') { ++argCount; }
			switch (argCount)
			{
			case 0:
				ID += c;
				break;
			case 1:
				lat += c;
				break;
			case 2:
				lonG += c;
				break;
			}
		}
		std::cout << lat << ' ' << isValidCoord(lat) << '\n';
		if (isValidCoord(lat) && isValidCoord(lonG))
		{
			cleanVersion += ID + lat + lonG + '\n';
		}
	}
	std::cout << cleanVersion;
	file.close();
	file.open(coor_file, std::ios::out);
	file << cleanVersion;
	file.close();
}
// checks if a string represented coordinate makes sense
bool RI::isValidCoord(std::string coord)
{
	bool hasDec = false, hasAlpha = false, hasDigits = false;
	bool (*isAllowed)(char) = [](char c) {
		for (char x : { 'N', 'W', '.', ' ', '-', '\t' })
		{
			if (c == x) { return true; }
		}
		if (isdigit(c)) { return true; }
		return false;
	};
	for (auto c : coord)
	{

		if (!isascii(c)) { return false; } else if (hasAlpha) { return false; } // should be last
		else if (!isAllowed(c)) { return false; } else if (c == '.' && hasDec) { return false; } else if (isalpha(c) && (hasAlpha || !hasDigits)) { return false; }

		else if (c == '.') { hasDec = true; } else if (isalpha(c)) { hasAlpha = true; } else if (isdigit(c)) { hasDigits = true; }
	}
	return hasAlpha && hasDigits;
}
// its a test wow
bool RI::isValidCoordUnitTest()
{
	struct TestCase {
		std::string in;
		bool out;
	};
	TestCase testSet[] =
	{
		{"42.333 W", true}, {"23.70 N", true},  {"-179.084 W", true},
		{"-0.2345 N", true}, {"W 2.4323", false},{ "23<27.5 N", false},
		{"54.56788 W ", false} , {"-23.09.09 N", false}, {"", false},
		{"23.567", false}, {"54.45\tN", true }, {"N", false}
	};
	for (TestCase tc : testSet) if (isValidCoord(tc.in) != tc.out)
	{
		std::cout <<
			"Test failed for isValidCoor!\nInput: " <<
			tc.in << "\nExpected: " << tc.out <<
			"\nResult: " << isValidCoord(tc.in) << '\n';
		return true;
	}
	std::cout << "Test passed for isValidCoor!\n";
	return false;
}
bool RI::runAllTests()
{
	RI test;
	return (test.isValidCoordUnitTest()) ? true : false;
}