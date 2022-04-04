#pragma once
#ifndef RAINLIST_H
#define NUMBERLIST_H
#include <iostream>

struct IDLookup 
{
    std::string str; // represents an ID, date, or other
    int num = 0; // represents an index inside an array
};

class RainList 
{
private:
    struct RainListNode
    {
        IDLookup value;         
        struct RainListNode* next;
    };
    RainListNode* head;
    IDLookup NULLSTRNUM = { "",-1 };
public:
    RainList() { head = nullptr; }
    ~RainList();
    void append(IDLookup);
    void insert(IDLookup);
    void remove(IDLookup);
    void displayList() const;
    bool isEmpty() const { return (!head); }
	bool hasValue(IDLookup)const;
    IDLookup get(std::string s);
};
#endif

