#include "RainHash.h"


RainHash::RainHash(int n)
{
	size = n;
	for (size_t i = 0; i < size; i++)
	{
		table.pushback(RainList());
	}
}
int RainHash::hash(std::string x)
{
	int n = 0, i = 2;
	for (char c : x) { n += (int(c) * i++); }
	if (!n) { return 0; } // avoid /0 error
	return n % size;
}
void RainHash::insert(IDLookup x)
{
	int hashvalue = hash(x.str);
	table.atReference(hashvalue)->append(x);
}
int RainHash::getnum(std::string s)
{
	int n = hash(s);
	return table.atReference(n)->get(s).num;
}
void RainHash::remove(std::string s)
{
	int n = hash(s);
	IDLookup foo = { s, 0 };
	table.atReference(n)->remove
	(
		foo	);
}
void RainHash::display()
{
	for (size_t i = 0; i < size; i++)
	{
		if (!table.atReference(i)->isEmpty())
		{
			table.atReference(i)->displayList();
		}
	}
}
// unit testing

void RainHashUnitTest::runTests()
{
	RainHash hasher(5);
	IDLookup testset[] =
	{ 
		{"ME", 0}, {"DOG", 5}, {"SNAKE", 3}, {"FISH", 7}, {"CAT", 41},
		{"LIZARD", 32}, {"RAT", 42}, {"ZEBRA", 42}, {"ROTWEILER", 96}
	};
	std::string removalset[] = { "DOG", "RAT", "CAT"};
	IDLookup keyset[] =
	{
		{"ME", 0}, {"DOG", -1}, {"SNAKE", 3}, {"FISH", 7}, {"CAT", -1},
		{"LIZARD", 32}, {"RAT", -1}, {"ZEBRA", 42}, {"ROTWEILER", 96}
	};
	for (IDLookup x : testset) { hasher.insert(x); }
	for (std::string s : removalset) { hasher.remove(s); }
	for (IDLookup x : keyset)
	{
		if (x.num != hasher.getnum(x.str))
		{
			std::cout <<
				"TEST FAILED\nExpected: " <<
				x.num << "\nResult: " << hasher.getnum(x.str) <<
				"\n For str: " << x.str << '\n';
		return;
		}
	}
	std::cout << "TEST PASSED\n";
}
//// basic constructor
//RainHash::RainHash(int n) : size(n)
//{
//	for (size_t i = 0; i < size; i++)
//	{
//		table.pushback(nullVal);
//		states.pushback(fresh);
//	}
//}
//// simple string based hasher
//int RainHash::hash(std::string ID)
//{
//	int n = 0, i = 2;
//	for (char c : ID) { n += (int(c) * i++); }
//	if (!n) { return 0; } // avoid /0 error
//	return n % size;
//}
//// inserts based on hash(x) with a linked list
//void RainHash::insert(idAndIndex x)
//{
//	int n = hash(x.ID);
//	table2.atReference(n)->append(x);
//
//	/*int n = hash(x.ID);
//	int start = n;
//	while (states.at(n) == inUse && table.at(n).ID != x.ID)
//	{
//		n++;
//		if (n == size) { n = 0; }
//		if (n == start) { return; }
//	}
//	if (table.at(n).ID == x.ID)
//	{
//		std::cout << x.ID << " already exists!\n";
//		return;
//	}
//	table.set(x, n);
//	states.set(inUse, n);*/
//}
//// finds based on hash(ID)
//int RainHash::search(std::string ID)
//{
//	int n = hash(ID);
//	idAndIndex foo = { ID, 0 };
//	if (table2.atReference(n)->hasValue
//	(
//		foo, 
//		[](idAndIndex x, idAndIndex y) {return (x.ID == y.ID); }
//	))
//	{
//		return n;
//	}
//	/*int start = n;
//	while (states.at(n) != fresh)
//	{
//		if (table.at(n).ID == ID) { return n; }
//		n++;
//		if (n == size) { n = 0; }
//		if (n == start) { return -1; }
//	}*/
//	return -1;
//}
//// unit tester
//void RainHash::runAllTests()
//{
//	std::cout << "Testing rain hash...\n";
//	if (!existsUnitTest())
//	{
//		std::cout << "Failed!\n";
//		return;
//	}
//	std::cout << "Passed!\n";
//}
//// removes item if it exists based on hash(x)
//void RainHash::remove(std::string ID)
//{
//	int num = search(ID);
//	idAndIndex foo = { ID, 0 };
//	if (num >= 0) if (table2.atReference(num)->hasValue(foo))
//	{
//		
//		/*table.set(nullVal, num);
//		states.set(used, num);*/
//	}
//}
//// more tests
//bool RainHash::existsUnitTest()
//{
//	RainHash hash(100);
//	std::string stringSet[] = 
//	{
//		"dog","cat","cat","bird","lizard","cow","person", "snake" 
//	};
//	std::string testSet[] = { "cow","frog", "person","snake" };
//	bool keySet[] = { true,false,true,false };
//	int i = 0;
//	for (std::string s : stringSet) { hash.insert({ s, 0 }); }
//	hash.remove("snake");
//	for (std::string s : testSet)
//	{
//		if (hash.exists(s) != keySet[i++])
//		{
//			std::cout << s << '\n';
//			return false;
//		}
//	}
//	return true;
//}
//int RainHash::getindex(std::string ID)
//{
//	int n = search(ID);
//	if (n >= 0) { return table.at(n).num; } else { return -1; }
//}