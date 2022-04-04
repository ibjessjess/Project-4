#pragma once
#ifndef LINKEDLIST_H
#define LINKDEFLIST_H
#include "Location.h"
#include <iostream>

// |!| NOT READY FOR USE
template <typename T>
class LinkNode {
private:
	char val;
	LinkNode* next;
public:
	LinkNode(T c);
	~LinkNode() { if (next) { delete next; } }
	void append(T c);
	void insert(T c);
	void rainInsert(RainDataEntry*);
	char getVal() { return val; }
	LinkNode* getNext() { return next; }
	
	// unit tests
	static void appendUnitTest();
	static void insertUnitTest();
};

#endif // !LINKEDLIST_H
