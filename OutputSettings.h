#pragma once
#ifndef OUTPUT_SETTINGS
#define OUTPUT_SETTINGS
#include "Location.h"

// some misc functions related to output that didn't seem appropriate
// for any existing class, or are used across many classes

bool const GRADED = 1; // toggles rubric specific features
const bool DEBUG = 0; // toggles debug statements
const int LINES_PER_SCREEN = 20; // lines displayed before requesting user input

inline void ScreenBreak(int i) { // used inside a for or while loop to pause output
	if (i % LINES_PER_SCREEN == 0 && i && GRADED) {
		system("pause");
		/*std::cout << "Press enter to continue\n";
		std::cin.get();*/
	}
}
inline void ScreenBreak(size_t i) {
	ScreenBreak(int(i));
}
inline std::string MonthFromInt(int n) {
	switch (n) {
	case 1: return "Jan";
	case 2: return "Feb";
	case 3: return "Mar";
	case 4: return "Apr";
	case 5: return "May";
	case 6: return "Jun";
	case 7: return "Jul";
	case 8: return "Aug";
	case 9: return "Sept";
	case 10: return "Oct";
	case 11: return "Nov";
	case 12: return "Dec";
	default: return "";
	}
}

#endif // !OUTPUT_SETTINGS