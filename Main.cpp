#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "UserInterpeter.h"
#include "Sorting.h"

// Zenyn, Jess, Germemias

int main() { UI::run(); }

/*	*****    READ ME    *****
* 
*   Project 3 Corrections
* 
*   Added descriptions above most functions
*   Some functions deemed self-explanatory were skipped
* 
*   UI::AddLocation() and UI::EditLocation() both now allow
*   the user to choose a region which already exists, or
*   type a brand new one
* 
*	Project 4 Requirementsa
*	
*	RainHash no longer uses linear probing, instead
*	it uses an array of linked lists. The list is defined
*   in RainList.h and implemented in RainList.cpp. It is
*   referenced throughout RainHash.h and RainHash.cpp
* 
*   Time complexity of linked list functions can be 
*   demonstrated using UI::linkedListDemo(). This function
*   cam be called be entering 'L' on the user menu
* 
*/