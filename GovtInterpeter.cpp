#include "GovtInterpeter.h"

/* Govt text file contains data that is directly copy pasted from spreadsheets,
*  which is messy but follows a predictable pattern. 6 unique types of lines exist,
*  and the following function determines what type of line is being examined */
void GovtFileInterpeter::incLineType(LineType& type,std::string& line) 
 {
	 int numTabs = 0;
	 switch (type) 
	 {
	 case null:
		 type = region;
		 break;
	 case region:
		 type = head1;
		 break;
	 case head1:
		 type = yearMonth;
		 break;
	 case yearMonth:
		 for (char c : line) if (c == '\t') { ++numTabs; }
		 if (numTabs > 1) { type = head2; }
		 break;
	 case head2:
		 type = entry;
		 break;
	 case entry:
		 for (char c : line) if (c == '\t') { ++numTabs; }
		 if (numTabs == 0) { type = region; }
		 break;
	}
}
/* This is the main function in charge of reading the text file. 
*  Everything else gets called from here */
 void GovtFileInterpeter::interpetGovtData( 
	 LocationArray& arr, std::string fileName
 ) {
	std::fstream file(fileName, std::ios::in);
	std::string line;
	std::string regionName;
	AnyArray<int> years;
	AnyArray<int> months;
	LineType curType = null;
	//size_t lineNum = 0; WARN:C4189 Initialized, not referenced - haven't used it
	//size_t STOP = 1000; WARN:C4189 Initialized, not referenced - haven't used it
	while (getline(file, line)) 
	{
		incLineType(curType, line);
		switch (curType) 
		{
		case region:
			years.clear();
			months.clear();
			regionName = line;
			for (char& c : regionName) 
			{
				if (c == ' ') { c = '_'; }
			}
			break;
		case head1:
			months.pushback(monthFromHead1(line));
			break;
		case yearMonth:
			readYearMonth(months, years, line);
			break;
		case head2:
			years.pushback(yearFromHead2(line));
			break;
		case entry:
			Location loc = readEntry(years, months, line, regionName);

			arr.pushback(loc);
			break;
		}
	}
	file.close();
}
 // reads an individual line from govt text file of 'entry' line type
Location GovtFileInterpeter::readEntry(
	 AnyArray<int>& years,
	 AnyArray<int>& months,
	 std::string line,
	 std::string regionName
 ) {
	 if (years.getSize() != months.getSize()) 
	 {
		 std::cout << "Years don't match months!\n";
		 throw;
	 }
	 std::string ID, name, rain;
	 RainDataEntry rainEntry;
	 size_t charCount = 0, monthYearCount = 0;
	 while (true) 
	 { 
		 ID.push_back(line[charCount++]);
		 if (line[charCount] == '\t') { break; }
	 }
	 charCount++;
	 while (true) 
	 {
		 name.push_back(line[charCount++]);
		 if (line[charCount] == '\t') { break; }
	 }
	 for (char& c : name) if (c == ' ') { c = '_'; }
	 charCount++;
	 Location loc(ID, name, regionName);
	 while (true) 
	 {
		 rain = "";
		 while (true) {
			 rain.push_back(line[charCount++]);
			 if (charCount >= line.length()) { return loc; }
			 if (line[charCount] == '\t') { break; }
		 }
		 charCount++;
		 if (charCount >= line.length()) { return loc; }
		 rainEntry = 
		 {
			 months[monthYearCount],
			 years[monthYearCount],
			 (isalpha(rain[0])) ? -1 : stod(rain)
		 };
		 loc.appendRainChart(rainEntry);
		 if (++monthYearCount >= months.getSize()) { return loc; }
	 }
	 return loc;
 }
 int GovtFileInterpeter::yearFromHead2(std::string line) 
 {
	 return stoi(line.substr(0, 4));
 }
 void GovtFileInterpeter::readYearMonth(
	 AnyArray<int>& months,
	 AnyArray<int>& years,
	 std::string line
 ) {
	 years.pushback(stoi(line.substr(0, 4)));
	 months.pushback(monthSTOI(line.substr(5, 3)));
 }
 int GovtFileInterpeter::monthFromHead1(std::string line)
 {
	 const size_t MONTH_POS = 2, MAX_CHARS_NEEDED = 3;
	 size_t tabs = 0;
	 for (size_t i = 0; i < line.length(); i++)
	 {
		 if (line[i] == '\t') { tabs++; }
		 else if (tabs == MONTH_POS)
		 {
			 return monthSTOI(line.substr(i, MAX_CHARS_NEEDED));
		 }
	 }
	 return -1; // exit
 }
 // gets an integer value of a month based on its string representation
 int GovtFileInterpeter::monthSTOI(std::string month) 
 {
	 // fail value = 0
	 switch (toupper(month[0])) 
	 {
	 case 'A':
		 switch (toupper(month[1]))
		 {
		 case 'P': return 4;
		 case 'U': return 8;
		 default: return 0; // fail
		 }
	 case 'D': return 12;
	 case 'F': return 2;
	 case 'J':
		 switch (toupper(month[1])) 
		 {
		 case 'A': return 1;
		 case 'U':
			 switch (toupper(month[2])) 
			 {
			 case 'L': return 7;
			 case 'N': return 6;
			 default: return 0;
			 }
		 default: return 0;
		 }
	 case 'M':
		 switch (toupper(month[2]))
		 {
		 case 'Y': return 5;
		 case 'R': return 3;
		 default: return 0;
		 }
	 case 'N': return 11;
	 case 'O': return 10;
	 case 'S': return 9;
	 default: return 0;
	 }
	 return 0;
 }