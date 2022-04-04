#include "Location.h"

Location::Location( // basic
  std::string id,
  std::string name,
  std::string region
) {
    this->ID = id;
    this->name = name;
    this->regionName = region;
}
// Basic constructor
Location::Location(std::string line) : latitude(99999), longitude(99999)
{ // single argument for optimized text 
    const int
        MAX_ARGS = 3,  // max arguments that may be processed
        ID_INDEX = 0,
        NAME_INDEX = 1,
        REGION_INDEX = 2;
    int i, argIndex = 0;
    std::string args[MAX_ARGS]; // argument array
    for (i = 0; i < MAX_ARGS; i++) args[i] = ""; // resets argument array
    for (i = 0; i < line.length(); i++) 
    {
        if (line[i] == ' ') { argIndex++; }
        else { args[argIndex] += line[i]; }
    }
    this->ID = args[ID_INDEX];
    this->name = args[NAME_INDEX];
    this->regionName = args[REGION_INDEX];
}
// double argument constructor for govt text
Location::Location(std::string line, int year) 
{
    const unsigned int
        MAX_ARGS = 18, // max arguments that may be processed
        ID_INDEX = 0, // indexes represent where arguments appear in text
        NAME_INDEX = 1,
        FIRST_MONTH_INDEX = 2,
        NUM_MONTHS = 12, // num months being processed per line (might change)
        CALENDAR_MONTHS = 12, // should not change in foreseeable future
        MONTH_OF_FIRST_ENTRY = 10; // |!| assumes first month is October
    unsigned int argCounter = 0, // determines which argument is being read
        i;
    RainDataEntry entry; // in govt data, rainfall is on same line as location
    std::string args[MAX_ARGS];
    for (i = 0; i < MAX_ARGS; i++) { args[i] = ""; } // loop to clear arguments
    for (size_t j = 0; j < line.length(); j++) // loop to fill arguments
    {
        if (line[j] == '\t') { ++argCounter; }
        if (argCounter >= MAX_ARGS) { break; }
        if (line[j] != ' ' && line[j] != '\t') { args[argCounter] += line[j]; }
    }
    setID(args[ID_INDEX]);
    setName(args[NAME_INDEX]);
    for (i = 0; i < NUM_MONTHS; ++i) // loop to push back rainfall entries
    {
        int month = MONTH_OF_FIRST_ENTRY + i;
        entry.month = ((month <= 12) ? month : (month % 12) + 1);
        entry.year = year + month / CALENDAR_MONTHS;
        try { entry.rain = std::stod(args[i + FIRST_MONTH_INDEX]); }
        catch (...) { entry.rain = -1; }
        rainChart.pushback(entry);
    }
}
// prints the important data of a location
void Location::printData()
{
	std::cout
		<< std::left << std::setw(10) << ID
		<< std::setw(25) << name << std::setw(20)
		<< ((hasRegion()) ? regionName : "unknown region") << '\n';
		//<< std::setw(10) << numEntries() << std::setw(5) << " entries\n";
}
// removes a specific entry via linear search
void Location::removeEntry(RainDataEntry entry) 
{
    for (int i = 0; i < numEntries(); i++) {
        if (rainChart.at(i).month == entry.month
            && rainChart.at(i).year == entry.year) { rainChart.remove(i); }
    }
}
// returns a string for out file purposes
std::string Location::outputEntry(int i) 
{
	std::string rain, month, year;
	if (getEntry(i).rain < 0) { rain = "      "; } // six spaces so lines stay even
	else { rain = std::to_string(getEntry(i).rain); }
	rain = rain.substr(0, 6);
	// adds a zero if month is single digit 
	if (getEntry(i).month < 10) {
		month = '0' + std::to_string(getEntry(i).month);
	} else { month = std::to_string(getEntry(i).month); }
	return month + '/' + std::to_string(getEntry(i).year) + ':' + rain + ' ';
}
// like print data but also prints rainfall entries
void Location::printMoreData() 
{ 
	printData();
	int size = rainChart.getSize();
	std::cout << "Month - Year - Precipitation(inches)\n";
	for (int i = 0; i < size; ++i)
	{
		ScreenBreak(i);
		std::cout
			<< std::left << std::setw(7)
			<< MonthFromInt(rainChart.at(i).month) << " " << rainChart.at(i).year
			<< "     " << ((rainChart.at(i).rain >= 0) ?
				std::to_string(rainChart.at(i).rain) : "unknown") << '\n';
	}
	printLocalExtrema();
}
// Merges two locations
Location Location::operator+(Location location) {// merges data from two Locations objs
	if (!hasID() && location.hasID()) this->ID = location.getID();
	if (!hasName() && location.hasName()) this->name = location.getName();
	if (!hasRegion() && location.hasRegion()) this->regionName = location.getRegion();
	for (int i = 0; i < location.rainChart.getSize(); i++)
	{
		// |!| does not check hasEntry(), always overrides
		rainChart.pushback(location.rainChart.at(i));
	}
	return *this;
}
// checks if entry for given month is present
bool Location::hasEntry(RainDataEntry entry) 
{ 
	for (int i = 0; i < numEntries(); i++)// does not check if actual data is present, just entry
	{
		if (rainChart.at(i).month == entry.month
			&& rainChart.at(i).year == entry.year) { return true; }
	}
	return false;
}
void Location::appendRainChart(std::string entry) 
{
	const int
		MONTH_INDEX = 0,
		MONTH_CHARS = 2,
		YEAR_INDEX = 3,
		YEAR_CHARS = 4,
		RAIN_INDEX = 8;
	RainDataEntry result;
	try
	{
		result.month = stoi(entry.substr(MONTH_INDEX, MONTH_CHARS));
		result.year = stoi(entry.substr(YEAR_INDEX, YEAR_CHARS));
		if (entry.length() > RAIN_INDEX) { result.rain = stod(entry.substr(RAIN_INDEX)); } else { result.rain = -1; }
		appendRainChart(result);
	} catch (...) {
		std::cout
		<< "Waning! bad data in in-file\n"
		<< "Entry: " << entry << '\n';
	}
}
// replaces entry if that month and year exist
void Location::replaceEntry(RainDataEntry entry)
{
	for (int i = 0; i < numEntries(); i++) // does not call hasEntry(), because that 
	{                                     // would require two linear searches instead of one
		if (rainChart.at(i).month == entry.month && rainChart.at(i).year == entry.year)
		{
			rainChart.set(entry, i);
			return;
		}
	}
}
// outputs min and max rainfall values
void Location::printLocalExtrema() const 
{
	RainDataEntry min, max;
	size_t i = 0, j;
	while (true)
	{ // set min and max with first valid entry
		if (rainChart.at(i).rain >= 0)
		{
			min = rainChart.at(i);
			max = rainChart.at(i);
			break;
		}
		++i;
	}
	for (j = i; j < rainChart.getSize(); j++) if (rainChart.at(j).rain >= 0)
	{
		if (rainChart.at(j).rain <= min.rain) { min = rainChart.at(j); }
		if (rainChart.at(j).rain >= max.rain) { max = rainChart.at(j); }

	}
	std::cout <<
		"The lowest recorded rainfall for this location occurred on " <<
		min.month << '/' << min.year << '\n' << min.rain << " inches was recorded that month\n"
		"The highest recorded rainfall for this location occurred on " <<
		max.month << '/' << max.year << '\n' << max.rain << " inches was recorded that month\n";
}
void Location::MergeRainCharts(Location loc) 
{
	for (size_t i = 0; i < loc.rainChart.getSize(); i++)
	{
		rainChart.pushback(loc.rainChart.at(i));
	}
}
// finds average rainfall for each month of a location
void Location::ComputeAvg()
{
	int const MONTHS = 12;
	double totalPerMonth[MONTHS];
	double entriesPerMonth[MONTHS];
	double foo;
	int i;

	// initialize arrays
	for (i = 0; i < MONTHS; i++)
	{
		totalPerMonth[i] = 0;
		entriesPerMonth[i] = 0;
		avgRainByMonth[i] = -1; // will represent unknown data point
	}
	for (i = 0; i < rainChart.getSize(); i++)
	{
		if (rainChart.at(i).rain >= 0)
		{
			totalPerMonth[rainChart.at(i).month - 1] += rainChart.at(i).rain;
			entriesPerMonth[rainChart.at(i).month - 1]++;
		}
	}
	for (i = 0; i < MONTHS; i++)
	{
		avgRainByMonth[i] = totalPerMonth[i] / entriesPerMonth[i];
	}
	std::cout << "Month Avg rain (inches)\n";
	for (i = 0; i < MONTHS; i++)
	{
		foo = avgRainByMonth[i];
		std::cout <<
			std::setw(9) << std::left << MonthFromInt(i + 1) <<
			((foo == -1) ? "unknown" : std::to_string(foo)) << "\n";
	}
}
void Location::setLatLong(double lat, double lonG) 
{
	latitude = lat;
	longitude = lonG;
}
void Location::printCoords() 
{
	std::cout << latitude << " W" << longitude << " N\n";
}



