#include "LocationArray.h"

// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

LocationArray::LocationArray() 
{ 
	arr = new Location[START_CAPACITY];
	maxLocations = START_CAPACITY;
	numLocations = 0;
}
// sets location argument to index
void LocationArray::set(Location location, int i, bool isUser) 
{ 
	arr[i] = location; 
	if (isUser) { sortLocations(); }
}
void LocationArray::reallocateRegions() 
{
	regions.reallocate();
	for (size_t i = 0; i < numLocations; i++)
	{
		regions.pushback(atReference((int)i));
	}
}
/* cleans up ID, name, and regions members to not 
*  include spaces or lowercase characters */
 void LocationArray::clean_location_members(int index)
{
	Location loc = at(index);
	std::string
		new_ID = loc.getID(),
		new_name = loc.getName(),
		new_region = loc.getRegion();

	for (size_t i = 0; i < new_ID.length(); i++)
	{
		if (islower(new_ID[i])) { new_ID[i] = (char)toupper(new_ID[i]); } else if (new_ID[i] == ' ') { new_ID[i] = '_'; }
	}
	for (size_t i = 0; i < new_name.length(); i++)
	{
		if (islower(new_name[i])) { new_name[i] = (char)toupper(new_name[i]); } else if (new_name[i] == ' ') { new_name[i] = '_'; }
	}
	for (size_t i = 0; i < new_region.length(); i++)
	{
		if (islower(new_region[i])) { new_region[i] = (char)toupper(new_region[i]); } else if (new_region[i] == ' ') { new_region[i] = '_'; }
	}
	loc.setID(new_ID);
	loc.setName(new_name);
	loc.setRegion(new_region);
	set(loc, index);
}
 /* Fills and sorts pointer arrays
 *  sortmode determines sort algo used, stop determines how many objects are used*/
void LocationArray::sortLocations(char sortmode, int stop)
{
	bool (*IDSort)(Location*, Location*) =
		[](Location* l1, Location* l2) { return (l1->getID() <= l2->getID()); };
	bool (*IDSortRev)(Location*, Location*) = 
		[](Location* l1, Location* l2) { return (l1->getID() > l2->getID()); };
	bool (*nameSort)(Location*, Location*) =
		[](Location* l1, Location* l2) { return (l1->getName() <= l2->getName()); };
	bool (*nameSortRev)(Location*, Location*) =
		[](Location* l1, Location* l2) { return (l1->getName() > l2->getName()); };
	bool (*entriesSort)(Location*, Location*) =
		[](Location* l1, Location* l2) { return (l1->numEntries() <= l2->numEntries()); };
	bool (*entriesSortRev)(Location*, Location*) =
		[](Location* l1, Location* l2) { return (l1->numEntries() > l2->numEntries()); };

	AlphaSortID.clear();
	ReverseAlphaSortID.clear();
	AlphaSortName.clear();
	ReverseAlphaSortName.clear();
	NumericSortNumEntries.clear();
	ReverseNumericSortNumEntries.clear();

	if (stop == -1) { stop = numLocations; }
	for (int i = 0; i < stop; i++)
	{
		clean_location_members(i);
		sortRainArray(i);
		AlphaSortID.pushback(atReference(i));
		ReverseAlphaSortID.pushback(atReference(i));
		AlphaSortName.pushback(atReference(i));
		ReverseAlphaSortName.pushback(atReference(i));
		NumericSortNumEntries.pushback(atReference(i));
		ReverseNumericSortNumEntries.pushback(atReference(i));
	}
	switch (sortmode)
	{
	case 's':
		selectionSort(AlphaSortID, IDSort);
		selectionSort(ReverseAlphaSortID, IDSortRev);
		selectionSort(AlphaSortName, nameSort);
		selectionSort(ReverseAlphaSortName, nameSortRev);
		selectionSort(NumericSortNumEntries, entriesSort);
		selectionSort(ReverseNumericSortNumEntries, entriesSortRev);
		break;
	case 'b':
		BubbleSort(AlphaSortID, IDSort);
		BubbleSort(ReverseAlphaSortID, IDSortRev);
		BubbleSort(AlphaSortName, nameSort);
		BubbleSort(ReverseAlphaSortName, nameSortRev);
		BubbleSort(NumericSortNumEntries, entriesSort);
		BubbleSort(ReverseNumericSortNumEntries, entriesSortRev);
		break;
	case 'm':
		MergeSort(AlphaSortID, IDSort);
		MergeSort(ReverseAlphaSortID, IDSort);
		MergeSort(AlphaSortName, nameSort);
		MergeSort(ReverseAlphaSortName, nameSortRev);
		MergeSort(NumericSortNumEntries, entriesSort);
		MergeSort(ReverseNumericSortNumEntries, entriesSortRev);
		break;
	case 'q':
		QuickSort(AlphaSortID, IDSort);
		QuickSort(ReverseAlphaSortID, IDSort);
		QuickSort(AlphaSortName, nameSort);
		QuickSort(ReverseAlphaSortName, nameSortRev);
		QuickSort(NumericSortNumEntries, entriesSort);
		QuickSort(ReverseNumericSortNumEntries, entriesSortRev);
	}
}
// uses selection search by ID to find an object matching the ID,
// or containing a leading substring which matches the ID.
// The array is then traversed up and down from the object found
// to find any other objects containing the initial substring,
// which is then returned as a dynamic array of location pointers
AnyArray<Location*> LocationArray::searchByID(std::string ID)
{
	AnyArray<Location*> result;
	bool (*equal)(Location*, std::string) = [](Location* loc, std::string id)
	{
		return equivalentPrefixes(id, loc->getID());
	};
	bool (*ordered)(Location*, std::string) = [](Location* loc, std::string id)
	{
		return (loc->getID() <= id);
	};
	size_t size = AlphaSortID.getSize(); // this line prevents subexpression overflow
	int index = SelectionSearch(AlphaSortID, ID, 0, (int)size - 1, equal, ordered);
	if (index >= 0 && index < numLocations)
	{
		result.pushback(AlphaSortID.at(index));
		// find all other results above index
		int i = index;
		while (true && 0 <= i && ++i < numLocations)
		{
			if (equal(AlphaSortID.at(i), ID)) { result.pushback(AlphaSortID.at(i)); } else { break; }
		}
		// find results below index
		i = index;
		while (true)
		{
			if (equal(AlphaSortID.at(--i), ID)) { result.pushback(AlphaSortID.at(i)); } else { break; }
		}
	}
	return result;
}
// prints all objects in order of ID
void LocationArray::printByID()
{
	for (size_t i = 0; i < AlphaSortID.getSize(); i++)
	{
		ScreenBreak(i);
		AlphaSortID.at(i)->printData();
	}
}
// prints all objects in order of name
void LocationArray::printbyName() 
{
	for (size_t i = 0; i < AlphaSortName.getSize(); i++)
	{
		ScreenBreak(i);
		AlphaSortName.at(i)->printData();
	}
}
#pragma warning(disable : 6385)
// appends an element to the end of an array
void LocationArray::pushback(Location location, bool isUser) 
{
	int n = hashMap.getnum(location.getID());
	if (n >= 0) // location already exists
	{ 
		atReference(n)->MergeRainCharts(location);
		return;
	}
	if (numLocations == maxLocations)
	{ // expands memory of array if needed
		Location* foo = new Location[maxLocations * GROWTH_RATE];
		for (int i = 0; i < maxLocations; i++) { foo[i] = arr[i]; }
		delete[] arr;
		arr = foo;
		maxLocations *= GROWTH_RATE;
		reallocateRegions();
	}
	arr[numLocations] = location;
	hashMap.insert({ location.getID(), numLocations });
	regions.pushback(&arr[numLocations]);
	numLocations++;
	if (isUser) { sortLocations(); }
}
// removes element at the specified index and reallocates
void LocationArray::remove(int index, bool isUser) 
{// removes an element from rainArray
	Location* foo = new Location[maxLocations];
	for (int i = 0; i < index; i++) { foo[i] = arr[i]; }
	for (int i = index + 1; i < numLocations; ++i) { foo[i - 1] = arr[i]; }
	delete[] arr;
	arr = foo;
	numLocations--;
	if (isUser) { sortLocations(); }
}
// returns the item at a given index
Location LocationArray::at(int i) const 
{ 
	if (i < numLocations) { return arr[i]; }
	throw std::runtime_error("Unexpected code path!"); // stay in range
}
// similar to at(), but returns a reference
Location* LocationArray::atReference(int i) const 
{
	if (i < numLocations) { return &arr[i]; }
	throw std::runtime_error("Unexpected code path!"); // stay in range
}
Region LocationArray::atRegion(int i) const 
{
	if (i < regions.getSize()) { return regions.at(i); }
	throw std::runtime_error("Unexpected code path!"); // stay in range
}
Region* LocationArray::atRegionReference(int i) const 
{
	if (i < regions.getSize()) { return regions.atReference(i); }
	throw std::runtime_error("Unexpected code path!"); // stay in range
}
// slow, stupid, linear search to find an index
int LocationArray::find(Location location) 
{
	for (int i = 0; i < numLocations; ++i) if (arr[i].getID() == location.getID()) { return i; }
	return -1;
}
LocationArray::~LocationArray() 
{ 
	regions.Deallocate(); // explicit deallocation of items before deallocation of array
	for (int i = 0; i < size(); i++) { at(i).Deallocate(); }
	delete[] arr;
}
// like size()
int LocationArray::numEntries() const 
{ 
	int result = 0;
	for (int i = 0; i < numLocations; i++) { result += at(i).numEntries(); }
	return result;
}
void LocationArray::printRegions() 
{
	std::string user;
	size_t size = regions.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ScreenBreak(i);
		std::cout <<
			i << ": " <<
			regions.at(i).getName() << '\n';
	}
}
// sorts rain array chronologically
void LocationArray::sortRainArray(int i)
{
	bool(*isOrdered)(RainDataEntry, RainDataEntry) =
		[](RainDataEntry r1, RainDataEntry r2) {
		return (r1.year > r2.year) ? true :
			(r1.year < r2.year) ? false :
			(r1.month > r2.month) ? true :
			(r1.month < r2.month) ? false : true;
	};
	QuickSort(atReference(i)->rainChart, isOrdered);
}
