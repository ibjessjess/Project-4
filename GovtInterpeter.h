#pragma once
#ifndef GOVT_INTERPETER_H
#define GOVT_INTERPETER_H
#include <iostream>
#include <fstream>
#include <string>
#include "LocationArray.h"
#include "RainHash.h"

// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

// Static name space style class used to interpret data from "GovtFile.txt"
// "GovtFile.txt" contains data copy pasted from tables found at 
// https://www.cnrfc.noaa.gov/rainfall_data.php and must be parsed in a
// specific way

class GovtFileInterpeter 
{
	GovtFileInterpeter() { /* static name space */ }
	const enum LineType 
	{ // different types of lines in order they appear
		null = 0,
		region, // "{region name}"
		head1, // "ID Location {first month}"
		yearMonth, // "{first year} {second month}"
		head2, // "{last year} {identifiers for further data points}"
		entry // "{ID} {name} {entries}"
	};
	static const int numLineTypes = 5;
	static void incLineType(LineType&, std::string&);
	static int monthFromHead1(std::string);
	static void readYearMonth(AnyArray<int>&, AnyArray<int>&, std::string);
	static int yearFromHead2(std::string);
	static Location readEntry(
		AnyArray<int>&, AnyArray<int>&,
		std::string, std::string);
public:
	static void interpetGovtData(LocationArray&, std::string);
	static int monthSTOI(std::string);
	static void fillJustLocs();
};

#endif // !GOVT_INTERPETER_H