#include "HashTable.h"
RainHash::RainHash(int n) {
	numBuckets = n;
	table = new RainArray<int>[numBuckets];
}
RainHash::~RainHash() {
	for (size_t i = 0; i < numBuckets; i++) {
		table[i].Deallocate();
	}
	delete[] table;
}
void RainHash::Insert(int key) {
	int i = Hash(key);
	table[i].pushback(key);
}
void RainHash::Remove(int key) {
	int i, index = Hash(key);
	for (i = 0; i < table[index].getSize(); i++) {
		if (table[index].at(i) == key) { break; }
	}
	if (i != table[index].getSize()) { table[index].remove(i); }
}
void RainHash::Print() {
	size_t i, j;
	for (i = 0; i < numBuckets; i++) {
		std::cout << "Bucket " << i << ": ";
		if (!table[i].getSize()) { std::cout << "empty"; }
		for (j = 0; j < table[i].getSize(); j++) {
			std::cout << table[i].at(j) << " ";
		}
		std::cout << '\n';
	}
}